<?php

\Vary\Router::get('/human', 'human', 'list');
\Vary\Router::get('/', 'default', 'list');
\Vary\Router::get('/Human', 'human', 'list');
\Vary\Router::get('/HUMAN/{id}', 'human', 'list');
var_dump(\Vary\Router::$__rules__);