/**
 * Module dependencies.
 */

var express = require('express');

var app = module.exports = express.createServer();

var cradle = require('cradle');
var couch = new (cradle.Connection)().database('toyblog');

couch.save('_design/posts', {
  all: {
         map: function(doc) {
                if (doc.content) emit(doc.content, doc);
              }
       }
});

// Configuration

app.configure(function(){
  app.set('views', __dirname + '/views');
  app.set('view engine', 'jade');
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
  app.use(express.static(__dirname + '/public'));
});

app.configure('development', function(){
  app.use(express.errorHandler({ dumpExceptions: true, showStack: true })); 
});

app.configure('production', function(){
  app.use(express.errorHandler()); 
});

// Routes

var setTitle = function(req, res, next) {
  req.title = 'My blog';
  next();
};

var loadAllPosts = function(req, res, next) {
  req.posts = [];
  couch.view('posts/all', function(err, docs) {
    req.posts = docs;
    console.log('req.posts is');
    console.log(req.posts);
    console.log(req.posts.length);
    next();
  });
};

app.get('/', setTitle, loadAllPosts, function(req, res){
  res.render('index', {
    posts: req.posts,
    title: req.title 
  });
});

app.get('/posts/new', setTitle, function(req, res) {
  res.render('new_post', {
    title: req.title  
  });
});

app.post('/posts/new', function(req, res) {
  couch.save(req.body.title, {
    description: req.body.description,
    content: req.body.content
  });

  res.redirect('/');
});

app.listen(3000);
console.log("Express server listening on port %d in %s mode", app.address().port, app.settings.env);
