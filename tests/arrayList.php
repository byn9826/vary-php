<?php
$array = new \Vary\ArrayList();
if ($array->push(2) !== true) {
  throw new Exception('ArrayList push error');
}
if ($array->unshift(1) !== true) {
  throw new Exception('ArrayList unshift error');
}
if ($array->push(3) !== true) {
  throw new Exception('ArrayList push error');
}
if ($array->size() !== 3) {
  throw new Exception('ArrayList size error');
}
if ($array->removeIndex(1) !== 2) {
  throw new Exception('ArrayList removeIndex error');
}
if ($array->pop() !== 3) {
  throw new Exception('ArrayList pop error');
}
if ($array->shift() !== 1) {
  throw new Exception('ArrayList shift error');
}
if ($array->size() !== 0) {
  throw new Exception('ArrayList size error');
}
$array = new \Vary\ArrayList(['a', 0, -1, true, null, false, 100]);
if ($array->size() !== 7) {
  throw new Exception('ArrayList size error');
}

$array1 = new \Vary\ArrayList();
if ($array1->size() !== 0) {
  throw new Exception('ArrayList1 initial error');
}
foreach([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]]
] as $case) {
  if ($array1->push($case) == false) {
    throw new Exception('ArrayList1 push error');
  }
}
if ($array1->size() !== 12) {
  throw new Exception('ArrayList1 push error');
}

$test2 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]] 
];
$array2 = new \Vary\ArrayList();
if ($array2->size() !== 0) {
  throw new Exception('ArrayList2 initial error');
}
foreach($test2 as $key => $case) {
  if ($array2->push($case) === false || $array2->size() - 1 !== $key) {
    throw new Exception('ArrayList2 push error');
  }
}
if ($array2->size() !== 12) {
  throw new Exception('ArrayList2 push error');
}

$array3 = new \Vary\ArrayList([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]] 
]);
if ($array3->size() !== count($array3->_items) || $array3->size() !== 12) {
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
  if ($array5->push($case) == false) {
    throw new Exception('ArrayList5 push error');
  }
  if ($array5->pop() !== $case) {
    throw new Exception('ArrayList5 pop error');
  }
  if ($array5->size() !== 0) {
    throw new Exception('ArrayList5 size error');
  }
}

$test6 = [0, 1, 2, null, true, false];
$array6 = new \Vary\ArrayList([0, 1, 2, null, true, false]);
foreach($test6 as $key => $case) {
  if ($array6->pop() !== $test6[count($test6) - $key - 1]) {
    throw new Exception('ArrayList6 pop error');
  }
}
if ($array6->size() !== 0) {
  throw new Exception('ArrayList6 size error');
}

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
  if ($array8->push($case) == false) {
    throw new Exception('ArrayList8 push error');
  }
  if ($array8->shift() !== $test8[$key] && $array8->size() !== 0) {
    throw new Exception('ArrayList8 shift error');
  }
}

$test9 = [0, 1, 2, null, true, false];
$array9 = new \Vary\ArrayList([0, 1, 2, null, true, false]);
foreach($test9 as $case) {
  if ($array9->shift() !== $case) {
    throw new Exception('ArrayList9 shift error');
  }
}
if ($array9->size() !== 0) {
  throw new Exception('ArrayList9 size error');
}

$test10 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\ArrayList(), [1, [new \Vary\ArrayList()]] 
];
$array10 = new \Vary\ArrayList();
if ($array10->size() !== 0) {
  throw new Exception('ArrayList10 size error');
}
foreach($test10 as $key => $case) {
  if ($array10->unshift($case) == false) {
    throw new Exception('ArrayList10 unshift error');
  }
  if ($array10->size() - 1 !== $key) {
    throw new Exception('ArrayList10 size error');
  }
}
foreach($test10 as $key => $case) {
  if ($array10->_items[11 - $key] != $case) {
    throw new Exception('ArrayList10 unshift error');
  }
}
if ($array10->size() !== 12) {
  throw new Exception('ArrayList10 size error');
}

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
if ($array12->size() !== 0) {
  throw new Exception('ArrayList12 size error');
}

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
if ($array13->pop() !== null) {
  throw new Exception('ArrayList13 pop error');
}
if ($array13->shift() !== 2) {
  throw new Exception('ArrayList13 pop error');
}
if ($array13->pop() !== 1) {
  throw new Exception('ArrayList13 pop error');
}
if ($array13->shift() !== 0) {
  throw new Exception('ArrayList13 pop error');
}


$stack = new \Vary\Stack();
if ($stack->push(1) !== true) {
  throw new Exception('Stack push error');
}
if ($stack->pop() !== 1) {
  throw new Exception('Stack pop error');
}
if ($stack->size() !== 0) {
  throw new Exception('Stack size error');
}
$stack = new \Vary\Stack([1, 2]);
if ($stack->size() !== 2) {
  throw new Exception('Stack size error');
}
if ($stack->pop() !== 2) {
  throw new Exception('Stack pop error');
}

$queue = new \Vary\Queue();
if ($queue->push(1) !== true) {
  throw new Exception('Queue push error');
}
if ($queue->shift() !== 1) {
  throw new Exception('Queue shift error');
}
$queue = new \Vary\Queue([1, 2]);
if ($queue->shift() !== 1) {
  throw new Exception('Queue shift error');
}
if ($queue->size() !== 1) {
  throw new Exception('Queue size error');
}

$deque = new \Vary\Deque();
if ($deque->push(1) !== true) {
  throw new Exception('Deque push error');
}
if ($deque->unshift(2) !== true) {
  throw new Exception('Deque unshift error');
}
if ($deque->size() !== 2) {
  throw new Exception('Deque size error');
}
if ($deque->shift() !== 2) {
  throw new Exception('Deque shift error');
}
if ($deque->pop() !== 1) {
  throw new Exception('Deque pop error');
}
if ($deque->size() !== 0) {
  throw new Exception('Deque size error');
}