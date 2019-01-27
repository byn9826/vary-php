<?php
$test1 = [54, 26, 93, 17, 77, 31, 44, 55, 20];
\Vary\Algorithm::shellSort($test1);
if ($test1 != [17, 20, 26, 31, 44, 54, 55, 77, 93]) {
  throw new Exception('shellSort1 error');
}

$test2 = [1, 2, 3, 2, -5];
\Vary\Algorithm::shellSort($test2);
if ($test2 != [-5, 1, 2, 2, 3]) {
  throw new Exception('shellSort2 error');
}

$test3 = [];
\Vary\Algorithm::shellSort($test3);
if ($test3 != []) {
  throw new Exception('shellSort3 error');
}