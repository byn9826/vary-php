<?php
$test = new \Vary\ArrayList([100, 10, -30, 0, -1, 1]);
\Vary\Algorithm::shellSort($test->value());
if ($test->value() != [-30, -1, 0, 1, 10, 100]) {
  throw new Exception('shellSort error');
}

$test1 = [54, 26, 93, 17, 77, 31, 44, 55, 20, -1, 0, 1, 2, -100];
\Vary\Algorithm::shellSort($test1);
if ($test1 != [-100, -1, 0, 1, 2, 17, 20, 26, 31, 44, 54, 55, 77, 93]) {
  throw new Exception('shellSort1 error');
}

$test2 = [];
\Vary\Algorithm::shellSort($test2);
if ($test2 != []) {
  throw new Exception('shellSort2 error');
}

$test3 = [1];
\Vary\Algorithm::shellSort($test3);
if ($test3 != [1]) {
  throw new Exception('shellSort3 error');
}

$test4 = [2, -2];
\Vary\Algorithm::shellSort($test4);
if ($test4 != [-2, 2]) {
  throw new Exception('shellSort4 error');
}

$test5 = [100, -11, 0, -1, '3', 'a', '44', '2', 1, 'b', 'abc', 'cc'];
\Vary\Algorithm::shellSort($test5);