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

$test6 = new \Vary\ArrayList([20, 10, 30, 100, 40, -20, 0, -10]);
\Vary\Algorithm::shellSort($test6->value(), function($a, $b) { return $a < $b; });
if ($test6->value()!= [-20, -10, 0, 10, 20, 30, 40, 100]) {
  throw new Exception('shellSort6 error');
}

$test7 = new \Vary\ArrayList(['a', 1, 0, -2, -3, -1, 2, 'aa', 'b', 'bb', 'c', 'aaa']);
\Vary\Algorithm::shellSort($test7->value(), function($a, $b) {
  if (is_long($a) && is_long($b)) { return $a < $b; }
  if (is_string($a) && is_string($b)) {
    return strlen($a) === strlen($b) ? $a < $b : strlen($a) < strlen($b);
  }
  return is_string($b);
});
if ($test7->value()!= [-3, -2, -1, 0, 1, 2, 'a', 'b', 'c', 'aa', 'bb', 'aaa']) {
  throw new Exception('shellSort7 error');
}

$test8 = new \Vary\ArrayList(['a', 2, 0, -1, 'b', 'aa', 1]);
\Vary\Algorithm::shellSort($test8->value(), function($a, $b) {
  if (is_long($a) && is_long($b)) { return $a < $b; }
  if (is_string($a) && is_string($b)) {
    return strlen($a) === strlen($b) ? $a < $b : strlen($a) < strlen($b);
  }
  return is_string($b);
});
if ($test8->value()!= [-1, 0, 1, 2, 'a', 'b', 'aa']) {
  throw new Exception('shellSort8 error');
}