require 'notifier'

module UrlTracker
  require_relative 'url_tracker/version'
  require_relative 'url_tracker/page'
  require_relative 'url_tracker/periodic'
  require_relative 'url_tracker/socket_communication'
  require_relative 'url_tracker/client'
  require_relative 'url_tracker/server'

  extend self

  require 'set'

  # Tracks +url+ fetching its content every +time+ seconds (defaults
  # to 5*60 - 5 minutes).
  def track_uri(uri, time = 5*60)
    init_ivars

    p = Page.new(URI(uri))
    
    if @pages.add?(p)
      @scheduler.task(uri).every(:minute) { check_change(p) }
      'ok'
    else
      'error'
    end
  end

  # Returns an array with all URIs currently being tracked
  def list_all
    init_ivars
    uri_list = @pages.map { |page| page.uri }

    uri_list.empty? ? ',' : uri_list.join(',')
  end

  # Stops tracking given URI
  def release_uri(uri)
    init_ivars

    p = Page.new(URI(uri))
    if @pages.include?(p)
      @pages.delete(p)
      @scheduler.remove_task(uri)
      'ok'
    else
      'error'
    end
  end

  # Forget about current tracked URIs
  def restart
    init_ivars
    @pages.clear
    @scheduler.restart
  end

private

  def check_change(page)
    Notifier.notify(title: 'Change!', message: "Page #{page.uri} changed!") if page.changed?
  end

  def init_ivars
    @pages      ||= Set.new
    @scheduler  ||= Periodic.new
  end
  
end
