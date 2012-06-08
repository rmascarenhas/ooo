require 'rake'

require './lib/url_tracker/version'

Gem::Specification.new do |s|
  s.name    = 'url_tracker'
  s.version = UrlTracker::VERSION

  s.summary     = 'A simple tool for tracking URLs and getting informed when they are changed.'
  s.description = 'A simple tool for tracking URLs and getting informed when they are changed.'

  s.author = 'Renato Mascarenhas'
  s.email  = 'renato.mascosta@gmail.com'

  s.files  = FileList['lib/*.rb', 'bin/*', 'test/*.rb'].to_a
  s.bindir = 'bin'
  
  s.executables << 'ut'
  s.executables << 'utd'

  s.add_runtime_dependency 'eventmachine'
  s.add_runtime_dependency 'libnotify'
  s.add_runtime_dependency 'notifier'

  s.license = 'MIT'
end
