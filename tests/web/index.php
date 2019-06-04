<?php

\Vary\Router::get('/', 'default', 'list');
\Vary\Router::get('/Human', 'human', 'list');
\Vary\Router::get('/human', 'human', 'test');
\Vary\Router::get('/human/{id}', 'human', 'get');
var_dump(\Vary\Router::$__rules__);

// var_dump($_SERVER['REQUEST_URI']);
// var_dump(\Vary\Router::handle($_SERVER['REQUEST_URI']));