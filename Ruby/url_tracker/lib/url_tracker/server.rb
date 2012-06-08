module UrlTracker

  require 'optparse'
  require 'ostruct'
  require 'logger'
  require 'pathname'

  # Class that waits for messages on a given socket and responds back to clients.
  # It can track URLs, list currently tracked URLs and stop tracking.
  # The interval between consecutive checks for change in the pages is customizable.
  class Server
    include SocketCommunication

    # Initializes a new server which logs its activities using the passed logger object.
    # Defaults to Ruby's Logger class. Log level defaults to +Logger::INFO+ and you can
    # override it by setting the URL_TRACKER_DEBUG environment variable
    def initialize(logger=Logger.new(STDERR))
      setup_signals
      @logger = logger
      @logger.level = level_from_env || Logger::INFO
    end

    # Main server method. Loops forever, waiting for new connections, and
    # checking new messages. Takes the appropriate action according to what is
    # received, such as a new URL to track.
    def loop_forever
      running = true

      while wait_for_connection
        @logger.info("New client connected")
        command, *arguments = next_message.split
        @logger.debug "#{command} received"
        response = case command
          when /^track$/i    then track(arguments.first)
          when /^list$/i     then list
          when /^release$/i  then release(arguments.first)
        end

        write(response) unless response.nil?
      end
    rescue => e
      @logger.error("An error occurred when waiting for new connections!\n\t#{e.inspect}\n\t#{e.backtrace.join("\n\t")}")
    end

    # Track an URL
    def track(uri)
      @logger.info("Tracking URL #{uri}")
      UrlTracker.track_uri(uri)
    end

    # List tracked URLs
    def list
      UrlTracker.list_all
    end

    # Release an URL
    def release(uri)
      @logger.info("Releasing URL #{uri}")
      UrlTracker.release_uri(uri)
    end

    # Runs the server, according to the argv options passed.
    # Possible options:
    #
    #   -s, --socket [FILE] #=> Uses FILE as socket file for communication
    #   -f, --fork          #=> Forks and works as a daemon
    #
    # +params+ can also be a hash, containing the parsed information to the server.
    def run(params)
      @logger.info "UrlTracker #{UrlTracker::VERSION} starting. Log level is #{@logger.level.inspect}."

      options = parse(params)
      @socket_file = options.socket_file
      @pid         = nil

      bind(@socket_file)

      @logger.info "Server starting at socket #{Pathname.new(@socket_file).realpath.to_s}"

      if options.fork
        @pid = fork { loop_forever }
        @logger.info "Forking to background. Child pid #{@pid}"
      else
        loop_forever
      end
    end

    # Stops the current server
    def stop
      @pid ? Process.kill('TERM', @pid) : close_connection
    end

  private

    def parse(argv)
      return OpenStruct.new(argv) if argv.kind_of?(Hash)

      options = OpenStruct.new
      options.socket_file = '/tmp/_ut.sock'
      options.fork        = false

      opts = OptionParser.new do |opts|
        opts.banner = 'Usage: utd [options]'
        opts.separator ''
        opts.separator 'Available options:'

        opts.on('-s', '--socket FILE', 'Uses FILE as a socket. Defaults to /tmp/_ut.sock') do |socket_file|
          options.socket_file = socket_file
        end

        opts.on('-f', '--fork', 'Forks and works as a daemon') do |f|
          options.fork = f
        end

        opts.on_tail('-h', '--help', 'Show this message') do
          puts opts
          exit(0)
        end
      end

      opts.parse!(argv)

      options
    end

    def setup_signals
      ['INT', 'TERM', 'QUIT'].each do |signal|
        Signal.trap(signal) do
          stop
          @logger.info "SIG#{signal} received. Bye."
          exit(0)
        end
      end
    end

    # Retrieves log level from DEBUG environment variable
    def level_from_env
      case ENV['URL_TRACKER_DEBUG']
        when 'debug'   then Logger::DEBUG
        when 'error'   then Logger::ERROR
        when 'fatal'   then Logger::FATAL
        when 'info'    then Logger::INFO
        when 'warning' then Logger::WARN
        else                nil
      end
    end

  end

end
