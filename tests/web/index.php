<?php

include_once(__DIR__ . '/Test.php');
$tests = Test::list();

if (count($tests) !== 3 || $tests[0]->id !== '1' || $tests[1]->id !== '2' || $tests[2]->id !== '3') {
  throw new Exception('Model list error');
}
if (Test::$__table__ !== 'test_connection' || \Vary\Model::$__table__ !== null) {
  throw new Exception('Model useTable error');
}
$tests = Test::list(1);
if (count($tests) !== 1 || $tests[0]->id !== '1') {
  throw new Exception('Model list error');
}
$tests = Test::list(2);
if (count($tests) !== 2 || $tests[0]->id !== '1' || $tests[1]->id !== '2') {
  throw new Exception('Model list error');
}
if (Test::$__columns__[0] !== 'id' || Test::$__columns__[1] !== 'name' || Test::$__columns__[2] !== 'note') {
  throw new Exception('Model useTable error');
}
