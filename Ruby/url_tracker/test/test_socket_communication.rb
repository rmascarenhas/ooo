require_relative 'test_helper'

class TestSocketCommunication < MiniTest::Unit::TestCase

  def setup
    @client = Object.new.extend(UrlTracker::SocketCommunication)
    @server = Object.new.extend(UrlTracker::SocketCommunication)
    @path    = '/tmp/.dk29ei39kd3.sock'
    remove_socket
  end

  def teardown
    [@client, @server].each { |o| o.close_connection }
  end

  def test_connect_to_valid_socket
    @server.bind(@path)
    assert @client.connect(@path), 'Should connect when given a valid socket file'
  end

  def test_raises_invalid_socket_with_bind_and_connect
    @server.bind(@path)

    assert_raises(UrlTracker::SocketCommunication::InvalidSocketError) do
      @server.connect(@path)
    end
  end

  def test_raises_invalid_socket_with_connect_and_bind
    @server.bind(@path)
    @client.connect(@path)

    assert_raises(UrlTracker::SocketCommunication::InvalidSocketError) do
      @client.bind(@path)
    end
  end

  def test_communication_client_to_server
    connect_server_and_client

    @client.write('message')

    assert_equal 'message', @server.next_message
  end

  def test_communication_server_to_client
    connect_server_and_client

    @server.write('message')

    assert_equal 'message', @client.next_message
  end

  def test_communication_bidirectional
    connect_server_and_client

    @server.write('server mesg')
    @client.write('client mesg')

    assert_equal 'client mesg', @server.next_message
    assert_equal 'server mesg', @client.next_message
  end

  def test_closing_client_connection_doesnt_remove_socket_file
    connect_server_and_client
    @client.close_connection

    assert File.exists?(@path), 'Expected socket file to exist when closing client connection'
  end

  def test_closing_server_connection_removes_socket_file
    connect_server_and_client
    @server.close_connection

    assert !File.exists?(@path), 'Expected socket file to be removed when server closes connection'
  end

  def remove_socket
    File.unlink @path if File.exists? @path
  end

  def connect_server_and_client
    @server.bind(@path)
    t = Thread.new { @server.wait_for_connection }
    @client.connect(@path)
    t.join # server is connected to client
  end

end
