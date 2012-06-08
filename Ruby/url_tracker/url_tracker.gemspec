require 'rake'

require './lib/url_tracker/version'

Gem::Specification.new do |s|
  s.name    = 'url_tracker'
  s.version = UrlTracker::VERSION

  s.summary     = 'A simple tool for tracking URLs and getting informed when they are changed.'
  s.description = 'A simple tool for tracking URLs and getting informed when they are changed.'

  s.author = 'Renato Mascarenhas'
  s.email  = 'renato.mascosta@gmail.com'
  s.homepage = 'https://github.com/rmascarenhas/ooo/tree/master/Ruby/url_tracker'

  s.files  = Dir['lib/**/*', 'bin/*', 'test/*']
  s.bindir = 'bin'
  
  s.executables << 'ut'
  s.executables << 'utd'

  s.add_runtime_dependency 'eventmachine'
  s.add_runtime_dependency 'libnotify'
  s.add_runtime_dependency 'notifier'

  s.license = 'MIT'
end
