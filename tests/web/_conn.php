<?php
\Vary\_conn::_setConn([
  'host' => '127.0.0.1',
  'port' => '3306',
  'database' => 'test',
  'username' => 'root',
  'password' => '123'
]);
$conn = \Vary\_conn::_getConn();
$conn1 = \Vary\_conn::_getConn();
if ($conn instanceof PDO !== true) {
  throw new Exception('_conn getConn error');
}
if ($conn1 instanceof PDO !== true) {
  throw new Exception('_conn getConn error');
}
if ($conn !== $conn1) {
  throw new Exception('_conn getConn error');
}
$test_conn = $conn->prepare("SELECT * FROM test_connection");
$test_conn->execute();
if (count($test_conn->fetchAll()) !== 3) {
  throw new Exception('_conn fetchAll error');
}
$test_conn1 = $conn->prepare("SELECT * FROM test_connection");
$test_conn1->execute();
$test_conn1 = $test_conn1->fetch();
if (
  $test_conn1['id'] !== '1'
  || $test_conn1['name'] !== 'test'
  || $test_conn1['note'] !== 'test1'
) {
  throw new Exception('_conn fetch error');
}

