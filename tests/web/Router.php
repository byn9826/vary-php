<?php

\Vary\Router::get('/', 'default', 'list');
\Vary\Router::get('/{name}', 'default', 'getName');
\Vary\Router::get('/Human', 'human', 'list');
\Vary\Router::get('/human', 'human', 'test');
\Vary\Router::get('/human/{id}', 'human', 'get');
\Vary\Router::get('/human/{type}/{id}', 'human', 'getTypeHuman');
\Vary\Router::post('/human', 'human', 'create');
\Vary\Router::put('/human/{id}', 'human', 'update');
\Vary\Router::patch('/human/{id}', 'human', 'replace');
\Vary\Router::delete('/human/{id}', 'human', 'delete');
$gets = \Vary\Router::$__get__;
$posts = \Vary\Router::$__post__;
$puts = \Vary\Router::$__put__;
$patches = \Vary\Router::$__patch__;
$deletes = \Vary\Router::$__delete__;
if (
  count($gets) !== 5
  || $gets['/'] !== ['default', 'list']
  || $gets['/{0}'] !== ['default', 'getName']
  || $gets['/human'] !== ['human', 'test']
  || $gets['/human/{0}'] !== ['human', 'get']
  || $gets['/human/{0}/{1}'] !== ['human', 'getTypeHuman']
  || $posts['/human'] !== ['human', 'create']
  || $puts['/human/{0}'] !== ['human', 'update']
  || $patches['/human/{0}'] !== ['human', 'replace']
  || $deletes['/human/{0}'] !== ['human', 'delete']
) {
  throw new Exception('Model add rule error');
};
var_dump(\Vary\Router::$__get__);
var_dump(\Vary\Router::handle('/'));
var_dump(\Vary\Router::handle('/human'));
var_dump(\Vary\Router::handle('/Human'));
var_dump(\Vary\Router::handle('/human/1'));
var_dump(\Vary\Router::handle('/human/A/2'));
var_dump(\Vary\Router::handle('/pet'));

// $ch = curl_init("localhost");
// curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "GET");
// curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
// $response = curl_exec($ch);
// curl_close($ch);
// var_dump($response);


// $ch = curl_init("localhost/human");
// curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "GET");
// curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
// $response = curl_exec($ch);
// curl_close($ch);
// var_dump($response);

// $ch = curl_init("localhost/human/1");
// curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "GET");
// curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
// $response = curl_exec($ch);
// curl_close($ch);
// var_dump($response);