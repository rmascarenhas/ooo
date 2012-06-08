require_relative 'test_helper'

class TestPage < MiniTest::Unit::TestCase

  def setup
    @page_fetcher = MiniTest::Mock.new
    @content = 'web page'

    # should receive method get with argument fake_url and return @content
    @page_fetcher.expect(:get, @content, [fake_url])

    @page = UrlTracker::Page.new(fake_url, @page_fetcher)
  end

  def test_page_content
    assert_equal @content, @page.content
  end

  def test_page_content_bang
    assert_equal @content, @page.content

    change_page_content('new content')
    assert_equal 'new content', @page.content!
  end

  def test_page_changed_before_cached_content
    assert !@page.changed?, 'Expected no changes when there is no cache of the page'
  end

  def test_page_changed_when_no_changes_happened
    @page.content # we now have a cache of the page

    assert !@page.changed?, 'Expected a false result when no changes occur to a page'
  end

  def test_page_changed_when_changes_really_happened
    @page.content
    change_page_content('new content')

    assert @page.changed?, 'Expected Page to indicate change when its content was changed'
  end

  def change_page_content(new_content)
    @page_fetcher.expect(:get, new_content, [fake_url])
  end

  def test_page_equality
    other_page = UrlTracker::Page.new(fake_url)
    assert_equal @page, other_page
  end

end
