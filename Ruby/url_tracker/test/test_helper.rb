require 'minitest/autorun'
require 'minitest/pride'
require_relative '../lib/url_tracker'

# messages sent via sockets in the tests cannot be longer than 100 bytes
MAX_TEST_SOCK_MESSAGE = 100 

puts "Url Tracker: running on Ruby version #{RUBY_VERSION}"

def assert_socket_received(message, socket)
  assert_block("Expected #{socket.inspect} to receive #{message.inspect}") do
    socket.recvfrom(MAX_TEST_SOCK_MESSAGE).first == message
  end
end

def async(stop = false, &block)
  @t = Thread.new(&block)
  @t.abort_on_exception = true

  while !@t.stop?; end if stop
end

def wait_sync
  @t.join
end

def finish_async
  @t.terminate
  @t.join
end

def remove_socket
  File.unlink(socket_file) if File.exists?(socket_file)
end

def socket_file
  '/tmp/.dk29ei39kd3.sock'
end

def fake_url
  'http://fake.com'
end
