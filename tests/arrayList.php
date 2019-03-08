<?php
$array = new \Vary\ArrayList();
if ($array->push(2) !== true) { throw new Exception('ArrayList push error'); }
if ($array->unshift(1) !== true) { throw new Exception('ArrayList unshift error'); }
if ($array->push(3) !== true) { throw new Exception('ArrayList push error'); }
if ($array->value() != [1, 2, 3]) { throw new Exception('ArrayList value error'); }
if ($array->index(0) !== 1) { throw new Exception('ArrayList index error'); }
if ($array->index(1) !== 2) { throw new Exception('ArrayList index error'); }
if ($array->index(2) !== 3) { throw new Exception('ArrayList index error'); }
if ($array->index(3) !== null) { throw new Exception('ArrayList index error'); }
if ($array->index(-1) !== null) { throw new Exception('ArrayList index error'); }
if ($array->indexOf(2) !== 1) { throw new Exception('ArrayList indexOf error'); }
if ($array->lastIndexOf(2) !== 1) { throw new Exception('ArrayList lastIndexOf error'); }
if ($array->removeIndex(1) !== 2) { throw new Exception('ArrayList removeIndex error'); }
if ($array->pop() !== 3) { throw new Exception('ArrayList pop error'); }
if ($array->shift() !== 1) { throw new Exception('ArrayList shift error'); }
if ($array->size() !== 0) { throw new Exception('ArrayList size error'); }
$array = new \Vary\ArrayList(['ab', 0, -1, 'ab', 0, -1, 100]);
if ($array->size() !== 7) { throw new Exception('ArrayList size error'); }
if ($array->indexOf('ab') !== 0) { throw new Exception('ArrayList indexOf error'); }
if ($array->lastIndexOf('ab') !== 3) {
  throw new Exception('ArrayList lastIndexOf error');
}
$array = new \Vary\ArrayList([1, -1, 0]);
$array->sort();
if ($array->value() !== [-1, 0, 1]) { throw new Exception('ArrayList sort error'); }
$array->sort(function($a, $b) { return $a > $b; });
if ($array->value() !== [1, 0, -1]) { throw new Exception('ArrayList sort error'); }
$array = new \Vary\ArrayList([1, 2]);
if ($array->concat([3, 4]) !== [1, 2, 3, 4]) {
  throw new Exception('ArrayList concat error');
}
if ($array->value() !== [1, 2]) { throw new Exception('ArrayList concat error'); }
if ($array->concat([]) !== [1, 2]) { throw new Exception('ArrayList concat error'); }
$array = new \Vary\ArrayList([1, 2, 3]);
if ($array->every(function($a) { return $a > 0; }) !== TRUE) {
  throw new Exception('ArrayList every error');
}
if ($array->every(function($a) { return $a > 1; }) !== FALSE) {
  throw new Exception('ArrayList every error');
}

$array1 = new \Vary\ArrayList();
if ($array1->size() !== 0) { throw new Exception('ArrayList1 initial error'); }
foreach([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]]
] as $case) {
  if ($array1->push($case) == false) { throw new Exception('ArrayList1 push error'); }
}
if ($array1->size() !== 12) { throw new Exception('ArrayList1 push error'); }

$test2 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]] 
];
$array2 = new \Vary\ArrayList();
if ($array2->size() !== 0) { throw new Exception('ArrayList2 initial error'); }
foreach($test2 as $key => $case) {
  if ($array2->push($case) === false || $array2->size() - 1 !== $key) {
    throw new Exception('ArrayList2 push error');
  }
}
if ($array2->size() !== 12) { throw new Exception('ArrayList2 push error'); }

$array3 = new \Vary\ArrayList([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]] 
]);
if (
  $array3->indexOf(0) !== 0 || $array3->indexOf(1) !== 1 || $array3->indexOf(2) !== 2
  || $array3->indexOf(null) !== 3 || $array3->indexOf(true) !== 4 || $array3->indexOf(false) !== 5
  || $array3->indexOf([]) !== 6 || $array3->indexOf([1]) !== 7 || $array3->indexOf([1, '1', false]) !== 8
  || $array3->indexOf([1, ['1']]) !== 9 || $array3->indexOf(-2) !== -1 || $array3->indexOf(3) !== -1
  || $array3->indexOf(new \Vary\ArrayList()) !== -1 || $array3->indexOf([1, [new \Vary\ArrayList()]]) !== -1
) {
  throw new Exception('ArrayList3 indexOf error');
}
if (
  $array3->lastIndexOf(0) !== 0 || $array3->lastIndexOf(1) !== 1 || $array3->lastIndexOf(2) !== 2
  || $array3->lastIndexOf(null) !== 3 || $array3->lastIndexOf(true) !== 4 || $array3->lastIndexOf(false) !== 5
  || $array3->lastIndexOf([]) !== 6 || $array3->lastIndexOf([1]) !== 7 || $array3->lastIndexOf([1, '1', false]) !== 8
  || $array3->lastIndexOf([1, ['1']]) !== 9 || $array3->lastIndexOf(-2) !== -1 || $array3->lastIndexOf(3) !== -1
  || $array3->lastIndexOf(new \Vary\ArrayList()) !== -1 || $array3->lastIndexOf([1, [new \Vary\ArrayList()]]) !== -1
) {
  throw new Exception('ArrayList3 lastIndexOf error');
}
if ($array3->size() !== count($array3->value()) || $array3->size() !== 12) {
  throw new Exception('ArrayList3 initial error');
}

$test4 = [0, 1, 2, null, true, false];
$array4 = new \Vary\ArrayList([0, 1, 2, null, true, false]);
if ($array4->size() !== 6) {
  throw new Exception('ArrayList4 initial error');
}
foreach($test4 as $key => $case) {
  if ($array4->pop() !== $test4[count($test4) - 1 - $key]) {
    throw new Exception('ArrayList4 pop error');
  }
}

$test5 = [0, 1, 2, null, true, false];
$array5 = new \Vary\ArrayList();
foreach($test5 as $key => $case) {
  if ($array5->push($case) == false) { throw new Exception('ArrayList5 push error'); }
  if ($array5->indexOf($case) !== 0) { throw new Exception('ArrayList5 indexOf error'); }
  if ($array5->lastIndexOf($case) !== 0) {
    throw new Exception('ArrayList5 lastIndexOf error');
  }
  if ($array5->pop() !== $case) { throw new Exception('ArrayList5 pop error'); }
  if ($array5->size() !== 0) { throw new Exception('ArrayList5 size error'); }
}

$test6 = [0, 1, 2, null, true, false];
$array6 = new \Vary\ArrayList([0, 1, 2, null, true, false]);
foreach($test6 as $key => $case) {
  if ($array6->pop() !== $test6[count($test6) - $key - 1]) {
    throw new Exception('ArrayList6 pop error');
  }
}
if ($array6->size() !== 0) { throw new Exception('ArrayList6 size error'); }

$test7 = [0, 1, 2, null, true, false];
$array7 = new \Vary\ArrayList([0, 1, 2, null, true, false]);
if ($array7->size() !== 6) {
  throw new Exception('ArrayList7 initial error');
}
foreach($test7 as $key => $case) {
  if ($array7->shift() !== $test7[$key]) {
    throw new Exception('ArrayList7 shift error');
  }
}

$test8 = [0, 1, 2, null, true, false];
$array8 = new \Vary\ArrayList();
foreach($test8 as $key => $case) {
  if ($array8->push($case) == false) { throw new Exception('ArrayList8 push error'); }
  if ($array8->shift() !== $test8[$key] && $array8->size() !== 0) {
    throw new Exception('ArrayList8 shift error');
  }
}

$test9 = [0, 1, 2, null, true, false];
$array9 = new \Vary\ArrayList([0, 1, 2, null, true, false]);
foreach($test9 as $case) {
  if ($array9->shift() !== $case) { throw new Exception('ArrayList9 shift error'); }
}
if ($array9->size() !== 0) { throw new Exception('ArrayList9 size error'); }

$test10 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]] 
];
$array10 = new \Vary\ArrayList();
if ($array10->size() !== 0) { throw new Exception('ArrayList10 size error'); }
foreach($test10 as $key => $case) {
  if ($array10->unshift($case) == false) {
    throw new Exception('ArrayList10 unshift error');
  }
  if ($array10->size() - 1 !== $key) {
    throw new Exception('ArrayList10 size error');
  }
}
foreach($test10 as $key => $case) {
  if ($array10->value()[11 - $key] != $case) {
    throw new Exception('ArrayList10 unshift error');
  }
}
if ($array10->size() !== 12) { throw new Exception('ArrayList10 size error'); }

$test11 = [0, 1, 2, null, true, false];
$array11 = new \Vary\ArrayList();
foreach($test11 as $key => $case) {
  if ($array11->unshift($case) == false) {
    throw new Exception('ArrayList11 unshift error');
  }
  if ($array11->shift() !== $test11[$key]) {
    throw new Exception('ArrayList11 shift error');
  }
  if ($array11->size() !== 0) {
    throw new Exception('ArrayList11 size error');
  }
}

$test12 = [0, 1, 2, null, true, false];
$array12 = new \Vary\ArrayList();
foreach($test12 as $key => $case) {
  if ($array12->unshift($case) == false) {
    throw new Exception('ArrayList12 unshift error');
  }
}
foreach($test12 as $key => $case) {
  if ($array12->shift() !== $test12[count($test12) - 1 - $key]) {
    throw new Exception('ArrayList12 shift error');
  }
}
if ($array12->size() !== 0) { throw new Exception('ArrayList12 size error'); }

$test13 = [0, 1, 2, null];
$array13 = new \Vary\ArrayList();
foreach($test13 as $key => $case) {
  if ($key === 0 || $key === 2) {
    if ($array13->unshift($case) === false) {
      throw new Exception('ArrayList13 unshift error');
    }
  } else {
    if ($array13->push($case) === false) {
      throw new Exception('ArrayList13 push error');
    }
  }
}
if ($array13->pop() !== null) { throw new Exception('ArrayList13 pop error'); }
if ($array13->shift() !== 2) { throw new Exception('ArrayList13 pop error'); }
if ($array13->pop() !== 1) { throw new Exception('ArrayList13 pop error'); }
if ($array13->shift() !== 0) { throw new Exception('ArrayList13 pop error'); }

$array14 = new \Vary\ArrayList([6, 2, -3, 0, 3, 1, 4, -4, 5, -1, -2]);
$array14->sort();
if ($array14->value() != [-4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6]) {
  throw new Exception('Array14 sort error');
}

$array15 = new \Vary\ArrayList([6, 2, -3, 0, 3, 1, 4, -4, 5, -1, -2]);
$array15->sort(function($a, $b) { return $a > $b; });
if ($array15->value() != [6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4]) {
  throw new Exception('Array15 sort error');
}

$array16 = new \Vary\ArrayList(['a', 2, 0, -1, 'b', 'aa', 1]);
\Vary\Algorithm::shellSort($array16->value(), function($a, $b) {
  if (is_long($a) && is_long($b)) { return $a < $b; }
  if (is_string($a) && is_string($b)) {
    return strlen($a) === strlen($b) ? $a < $b : strlen($a) < strlen($b);
  }
  return is_string($b);
});
if ($array16->value()!= [-1, 0, 1, 2, 'a', 'b', 'aa']) {
  throw new Exception('Array16 sort error');
}

$array17 = new \Vary\ArrayList([1, null, false, 'a', 'bb']);
if ($array17->concat([123]) !== [1, null, false, 'a', 'bb', 123]) {
  throw new Exception('ArrayList17 concat error');
}
if ($array17->value() !== [1, null, false, 'a', 'bb']) {
  throw new Exception('ArrayList17 concat error');
}
if ($array17->push('ccc') !== true) { throw new Exception('ArrayList17 push error'); }
if ($array17->index($array17->size() - 1) !== 'ccc') {
  throw new Exception('ArrayList17 push error');
}

$stack = new \Vary\Stack();
if ($stack->push(1) !== true) { throw new Exception('Stack push error'); }
if ($stack->pop() !== 1) { throw new Exception('Stack pop error'); }
if ($stack->size() !== 0) { throw new Exception('Stack size error'); }
$stack = new \Vary\Stack([1, 2]);
if ($stack->value() !== [1, 2]) { throw new Exception('Stack value error'); }
if ($stack->index(0) !== 1) { throw new Exception('Stack index error'); }
if ($stack->pop() !== 2) { throw new Exception('Stack pop error'); }

$queue = new \Vary\Queue();
if ($queue->push(1) !== true) { throw new Exception('Queue push error'); }
if ($queue->shift() !== 1) { throw new Exception('Queue shift error'); }
if ($queue->value() !== []) { throw new Exception('Queue value error'); }
$queue = new \Vary\Queue([1, 2]);
if ($queue->shift() !== 1) { throw new Exception('Queue shift error'); }
if ($queue->size() !== 1) { throw new Exception('Queue size error'); }
if ($queue->index(0) !== 2) { throw new Exception('Queue index error'); }

$deque = new \Vary\Deque();
if ($deque->push(1) !== true) { throw new Exception('Deque push error'); }
if ($deque->unshift(2) !== true) { throw new Exception('Deque unshift error'); }
if ($deque->value() !== [2, 1]) { throw new Exception('Deque value error'); }
if ($deque->index(1) !== 1) { throw new Exception('Deque index error'); }
if ($deque->shift() !== 2) { throw new Exception('Deque shift error'); }
if ($deque->pop() !== 1) { throw new Exception('Deque pop error'); }
if ($deque->size() !== 0) { throw new Exception('Deque size error'); }