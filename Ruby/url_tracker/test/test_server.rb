require_relative 'test_helper'

class TestServer < MiniTest::Unit::TestCase

  def setup
    remove_socket
    @server = UrlTracker::Server.new(Logger.new('/dev/null'))

    async(true) { @server.run(socket_file: socket_file) }

    @client = UrlTracker::Client.new(socket_file) # connected!
  end

  def teardown
    @client.close_connection
    finish_async
    @server.close_connection
    UrlTracker.restart
  end

  def test_successfully_tracks_url
    assert @client.track(fake_url), 'Expected to be able to track new URI'
  end

  def test_track_same_url
    @client.track(fake_url)
    restart_client
  
    assert !@client.track(fake_url), 'Expected to return false when tracking same URI'
  end

  def test_list_empty
    assert_equal [], @client.list
  end

  def test_list_with_single_uri
    @client.track(fake_url)
    restart_client

    assert_equal [fake_url], @client.list
  end

  def test_list_two_uris
    @client.track(fake_url)
    restart_client
    @client.track(fake_url + '/fake')
    restart_client

    assert_equal [fake_url, fake_url + '/fake'], @client.list
  end

  def test_release_when_tracking_nothing
    assert !@client.release(fake_url), 'Expected to return false when releasing not tracked URI'
  end

  def test_release_with_tracked_uri
    @client.track(fake_url)
    restart_client
    assert @client.release(fake_url), 'Expected to release tracked URI'
    
    restart_client
    assert_equal [], @client.list
  end

  def restart_client
    @client.close_connection

    @client = UrlTracker::Client.new(socket_file)
  end

end
