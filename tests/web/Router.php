<?php

\Vary\Router::get('/', 'default', 'list');
\Vary\Router::get('/{name}', 'default', 'getName');
\Vary\Router::get('/Human', 'human', 'list');
\Vary\Router::get('/human', 'human', 'test');
\Vary\Router::get('/human/{id}', 'human', 'get');
\Vary\Router::get('/human/{type}/{id}', 'human', 'getTypeHuman');
$rules = \Vary\Router::$__rules__;
if (
  count($rules) !== 5
  || $rules['/'] !== ['default', 'list']
  || $rules['/{0}'] !== ['default', 'getName']
  || $rules['/human'] !== ['human', 'test']
  || $rules['/human/{0}'] !== ['human', 'get']
  || $rules['/human/{0}/{1}'] !== ['human', 'getTypeHuman']
) {
  throw new Exception('Model create error');
};
var_dump(\Vary\Router::$__rules__);
var_dump(\Vary\Router::handle('/'));
var_dump(\Vary\Router::handle('/human'));
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