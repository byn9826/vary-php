<?php

include_once(__DIR__ . '/Test.php');
$tests = Test::list();
if (count($tests) !== 3 || $tests[0]->id !== '1' || $tests[1]->id !== '2' || $tests[2]->id !== '3') {
  throw new Exception('Model list error');
}
if (Test::$__table__ !== 'test_connection' || \Vary\Model::$__table__ !== null) {
  throw new Exception('Model useTable error');
}
if (Test::$__primary__ !== 'id' || \Vary\Model::$__primary__ !== null) {
  throw new Exception('Model usePrimary error');
}
if (Test::$__columns__[0] !== 'id' || Test::$__columns__[1] !== 'name' || Test::$__columns__[2] !== 'note') {
  throw new Exception('Model useTable error');
}
$tests = Test::list([
  'limit' => 1
]);
if (count($tests) !== 1 || $tests[0]->id !== '1') {
  throw new Exception('Model list error');
}
$tests = Test::list([
  'limit' => 2
]);
if (count($tests) !== 2 || $tests[0]->id !== '1' || $tests[1]->id !== '2') {
  throw new Exception('Model list error');
}
$tests[0]->name = 'new name';
$tests[0]->note = 'new note';
if ($tests[0]->__origin__ !== ['id' => '1', 'name' => 'test', 'note' => 'test1']) {
  throw new Exception('Model list error');
}
if ($tests[1]->__origin__ !== ['id' => '2', 'name' => 'test2', 'note' => 'haha']) {
  throw new Exception('Model list error');
}
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$test_valid = Test::list([
  'limit' => 1
]);
if (
  $test_valid[0]->id !== '1'
  || $test_valid[0]->name !== 'new name'
  || $test_valid[0]->note !== 'new note'
) {
  throw new Exception('_conn list error');
}
$tests[0]->name = 'test';
$tests[0]->note = 'test1';
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$test_valid = Test::list([
  'limit' => 1
]);
if (
  $test_valid[0]->id !== '1'
  || $test_valid[0]->name !== 'test'
  || $test_valid[0]->note !== 'test1'
) {
  throw new Exception('Model update error');
}

$tests = Test::list([
  'limit' => 1,
  'offset' => 0
]);
if (
  $tests[0]->id !== '1'
  || $tests[0]->name !== 'test'
  || $tests[0]->note !== 'test1'
) {
  throw new Exception('Model list error');
}

$tests = Test::list([
  'limit' => 2,
  'offset' => 1
]);
if (
  $tests[0]->id !== '2'
  || $tests[0]->name !== 'test2'
  || $tests[0]->note !== 'haha'
  || $tests[1]->id !== '3'
  || $tests[1]->name !== 'test3'
  || $tests[1]->note !== 'lalala'
) {
  throw new Exception('Model list error');
}
