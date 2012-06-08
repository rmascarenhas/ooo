require 'net/http'

module UrlTracker

  # Class representing a single web page to be tracked. It is capable of fetching
  # the page's content and verifying if it was changed since last time it was fetched
  class Page
    attr_reader :uri

    # Creates a new instance of UrlTracker::Page. The first argument is the URI that
    # corresponds to the page and will be lazily fetched. The second parameter 
    # is an object that is responsible for fetching the page itself. It mus respond
    # to the +get+ method with the given +uri+ parameter and return a string
    # with the page contents; this parameter defaults to +Net::HTTP+, so you should
    # by default pass +uri+ as an instance of +URI::Generic+
    def initialize(uri, page_fetcher = Net::HTTP)
      @uri          = uri.dup
      @page_fetcher = page_fetcher
    end

    # Returns a string containing the page content. If not yet fetched, this method
    # will fetch the page for you.
    def content
      @content ||= fetch
    end

    # This method returns a string containing the page content, but always fetches
    # the page again
    def content!
      @content = fetch
    end

    # Verifies if a page has changed since last the last time it was fetched
    def changed?
      if @content # we have a cached copy
        old_content = @content
        @content = fetch
        @content != old_content
      else
        @content = fetch
        false
      end
    end

    # Two pages are considered the same if they have the same URI. Right, that
    # might not be true if the content is different (which shouldn't if you are
    # building a RESTful service), but we will just ignore that and pretend we
    # we live in a better world.
    def eql?(other)
      @uri.eql?(other.uri)
    end

    def ==(other)
      @uri == other.uri
    end

    def hash
      @uri.hash
    end

  private

    def fetch
      @page_fetcher.get(@uri)
    end

  end

end
