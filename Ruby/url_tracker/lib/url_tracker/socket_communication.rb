module UrlTracker

  require 'socket'

  # Implements communication via Unix sockets.
  module SocketCommunication
    attr_writer :path # path to the socket file

    InvalidSocketError = Class.new(StandardError)

    # Messages received cannot be longer than 1024 bytes
    MAX_MESSAGE_LENGTH = 1024

    # Max connections to be queued before accept
    MAX_CONN_QUEUE = 10

    # Connects to the Unix socket. Returns true if the connection was successful.
    # Otherwise an exception is thrown. This method cannot be called if +bind+ was
    # already called; neither can you call +bind+ if you call this method.
    def connect(path)
      raise_socket_error_if { defined? @socket }
      @socket = Socket.new(:UNIX, :SOCK_STREAM, 0)
      @socket.connect addrinfo_for(path)
      true
    end

    # Binds the given path, creating a Unix socket. As with connect, you cannot use
    # this method if already called +connect+ before. After this method is called,
    # the socket will be waiting for connections.
    def bind(path)
      raise_socket_error_if { defined? @socket }
      @socket_file = path
      @socket = Socket.new(:UNIX, :SOCK_STREAM, 0)
      @socket.bind addrinfo_for(@socket_file)
      @socket.listen(MAX_CONN_QUEUE)
      true
    end

    # Waits for a connection in the binded socket
    def wait_for_connection
      @current_client = @socket.accept.first
    end

    # Writes to the socket, returning the number of bytes sent. This method can
    # only be called before +connect+ or +wait_for_connection+, otherwise you
    # will get an exception
    def write(message)
      socket = (defined? @current_client) ? @current_client : @socket
      socket.send(message, 0)
    end

    # Waits for a message. Blocks until it is received.
    def next_message
      socket = (defined? @current_client) ? @current_client : @socket
      socket.recvfrom(MAX_MESSAGE_LENGTH).first
    end

    def close_connection
      defined?(@socket) && !@socket.closed? && @socket.close
      File.unlink(@socket_file) if defined?(@socket_file) && File.exists?(@socket_file)
    end

  private

    def raise_socket_error_if(&block)
      raise InvalidSocketError if block.call
    end

    def addrinfo_for(path)
      Addrinfo.unix(path)
    end

  end

end
