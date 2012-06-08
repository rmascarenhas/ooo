url\_tracker
============

Simple tool to watch for changes in URLs and get notified.
Just a toy I built to play a bit. Might be useful to someone.

Instalation
-----------

    $ gem install url_tracker

Start the server

    $ utd

Usage
-----

### Track a new URL

    $ ut -t mywebsite.com

UrlTracker will look for changes in the website every minute. There is no way to change
it for now (unless you change the source code).

### Release an URL (stop tracking)

    $ ut -r mywebsite.com

Both commands have a `-h` flag with available options.

Caveats
-------

There is currently no way to change the default period in which the tool will look for changes
on pages (unless you edit the source code). I may or may not change this. This was a toy I built
for fun, after all (that's why it is in this repository). However, if you like it, pull request me.

License
-------

MIT Licese. See LICENSE file.
