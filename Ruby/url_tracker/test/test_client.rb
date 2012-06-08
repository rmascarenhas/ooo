require_relative 'test_helper'

class TestClient < MiniTest::Unit::TestCase

  def setup
    remove_socket
    @server = Object.new.extend(UrlTracker::SocketCommunication)
    @server.bind(socket_file)

    async do
      @server.wait_for_connection
    end

    @client = UrlTracker::Client.new(socket_file)

    wait_sync
  end

  def teardown
    @client.close_connection
    @server.close_connection
  end

  def test_successful_track_new_url
    async { @success = @client.track(fake_url) }
    @server.write('ok')
    wait_sync

    assert_equal "track #{fake_url}", @server.next_message
    assert @success
  end

  def test_server_could_not_track_url
    async { @success = @client.track(fake_url) }
    @server.write('error')
    wait_sync

    assert !@success
  end

  def test_list_urls_empty
    async { @urls = @client.list }
    @server.write(',')
    wait_sync

    assert_empty @urls
  end

  def test_list_single_url
    async { @urls = @client.list }
    @server.write(fake_url)
    wait_sync

    assert_equal [fake_url], @urls
  end

  def test_list_multiple_urls
    async { @urls = @client.list }
    @server.write(fake_url + ',http://foo.bar,http://google.com')
    wait_sync

    assert_equal [fake_url, 'http://foo.bar', 'http://google.com'], @urls
  end

  def test_release_tracked_url
    async { @client.track(fake_url) }
    @server.write('ok')
    wait_sync

    @server.next_message # so that we don't get the tracking request on the assertion

    async { @success = @client.release(fake_url) }
    @server.write('ok')
    wait_sync

    assert_equal "release #{fake_url}", @server.next_message
    assert @success
  end

  def test_release_url_not_tracked
    async { @success = @client.release(fake_url) }
    @server.write('error')
    wait_sync

    assert !@success
  end

end
