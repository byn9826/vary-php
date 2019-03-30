<?php

$result1 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 3);
if ($result1['exist'] !== true) { throw new Exception('binarySearch1 exist error'); }
if ($result1['index'] !== 3) { throw new Exception('binarySearch1 index error'); }

$result2 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], -1);
if ($result2['exist'] !== true) { throw new Exception('binarySearch2 exist error'); }
if ($result2['index'] !== 0) { throw new Exception('binarySearch2 index error'); }

$result3 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], -2);
if ($result3['exist'] !== false) { throw new Exception('binarySearch3 exist error'); }
if ($result3['index'] !== 0) { throw new Exception('binarySearch3 index error'); }

$result4 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], -5);
if ($result4['exist'] !== false) { throw new Exception('binarySearch2 exist error'); }
if ($result4['index'] !== 0) { throw new Exception('binarySearch2 index error'); }

$result5 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 12);
if ($result5['exist'] !== true) { throw new Exception('binarySearch5 exist error'); }
if ($result5['index'] !== 9) { throw new Exception('binarySearch5 index error'); }

$result6 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 13);
if ($result6['exist'] !== false) { throw new Exception('binarySearch6 exist error'); }
if ($result6['index'] !== 10) { throw new Exception('binarySearch6 index error'); }

$result7 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 7);
if ($result7['exist'] !== true) { throw new Exception('binarySearch7 exist error'); }
if ($result7['index'] !== 6) { throw new Exception('binarySearch7 index error'); }

$result8 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 8);
if ($result8['exist'] !== false) { throw new Exception('binarySearch8 exist error'); }
if ($result8['index'] !== 7) { throw new Exception('binarySearch8 index error'); }

$result9 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 9);
if ($result9['exist'] !== false) { throw new Exception('binarySearch9 exist error'); }
if ($result9['index'] !== 7) { throw new Exception('binarySearch9 index error'); }

$result10 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 10);
if ($result10['exist'] !== true) { throw new Exception('binarySearch10 exist error'); }
if ($result10['index'] !== 7) { throw new Exception('binarySearch10 index error'); }


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