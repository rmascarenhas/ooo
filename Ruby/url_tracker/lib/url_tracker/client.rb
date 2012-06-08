module UrlTracker

  # Class who deals with requesting information to the server, such as
  # track a new URL, list all currently tracked links, stop tracking something, etc.
  class Client
    include SocketCommunication

    require 'optparse'
    require 'ostruct'

    def initialize(socket_file = '/tmp/_ut.sock')
      connect(socket_file)
    rescue Errno::ENOENT
      STDERR.puts 'Connection error. Is the server running?'
      exit(1)
    end

    # Sends a message to the server asking to track a new URL. Format
    # of the message:
    #
    #   "track {{URL}}"
    def track(url)
      write("track #{url}")
      next_message == 'ok'
    end

    # Asks the server for all the URLs currently being tracked. Expects a
    # string back, with URLs separated by commas.
    def list
      write('list')
      next_message.split(',')
    end

    # Tells the server to stop tracking the given URL. Returns true if the
    # operation was successful
    def release(url)
      write("release #{url}")
      next_message == 'ok'
    end

    # Tells the server to shutdown
    def shutdown
      write('shutdown')
    end

    # Calls one of the methods above according to the options passed.
    # Available options:
    #
    #   -t, --track URL   #=> Starts tracking URL
    #   -l, --list        #=> List currently tracked URLs
    #   -r, --release URL #=> Releases URL, not tracking it any more
    #
    # +params+ can also be a hash, in which case it will be considered already parsed.
    def run(params)
      options = parse(params)

      output = case options.action
        when :track   then track(options.url)
        when :list    then list
        when :release then release(options.url)
      end
    end


  private

    def parse(argv)
      return OpenStruct.new(argv) if argv.kind_of?(Hash)

      options = OpenStruct.new
      options.action = :nothing

      opts = OptionParser.new do |opts|
        opts.banner = 'Usage: ut [options]'
        opts.separator ''
        opts.separator 'Available options:'

        opts.on('-t', '--track URL', 'Start tracking URL') do |url|
          options.url    = prepare_url(url)
          options.action = :track
        end

        opts.on('-l', '--list', 'List currently tracked URLs') do |list|
          options.action = :list
        end

        opts.on('-r', '--release URL', 'Release URL, not tracking it any more') do |url|
          options.url    = url
          options.action = :release
        end

        opts.on_tail('-h', '--help', 'Show this message') do
          puts opts
          exit(0)
        end
      end

      opts.parse!(argv)

      options
    end

    def prepare_url(url)
      url.tap { url.prepend('http://') unless url.start_with?('http://') }
    end

  end

end
