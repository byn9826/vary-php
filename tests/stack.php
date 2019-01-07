<?php
$test_array1 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Stack(), [1, [new \Vary\Stack()]] 
];
$stack1 = new \Vary\Stack();
if ($stack1->size() !== 0) {
  throw new Exception('Stack1 initial error');
}
foreach($test_array1 as $case) {
  if ($stack1->push($case) == false) {
    throw new Exception('Stack 1 push error');
  }
}
if ($stack1->size() !== 12) {
  throw new Exception('Stack1 size error');
}

$test_array2 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Stack(), [1, [new \Vary\Stack()]] 
];
$stack2 = new \Vary\Stack();
if ($stack2->size() !== 0) {
  throw new Exception('Stack2 initial error');
}
foreach($test_array2 as $key => $case) {
  if ($stack2->push($case) == false) {
    throw new Exception('Stack2 push error');
  }
  if ($stack2->size() - 1 !== $key) {
    throw new Exception('Stack2 size error');
  }
}
if ($stack2->size() !== 12) {
  throw new Exception('Stack2 final error');
}

$stack3 = new \Vary\Stack([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Stack(), [1, [new \Vary\Stack()]] 
]);
if ($stack3->size() !== count($stack2->_items)) {
  throw new Exception('Stack3 initial error');
}
if ($stack3->size() !== 12) {
  throw new Exception('Stack3 final error');
}

$test_array4 = [0, 1, 2, null, true, false];
$stack4 = new \Vary\Stack([0, 1, 2, null, true, false]);
if ($stack4->size() !== 6) {
  throw new Exception('Stack4 initial error');
}
foreach($test_array4 as $key => $case) {
  $return = $stack4->pop();
  if ($return !== $test_array4[count($test_array4) - 1 - $key]) {
    throw new Exception('Stack4 pop error');
  }
}

$test_array5 = [0, 1, 2, null, true, false];
$stack5 = new \Vary\Stack();
foreach($test_array5 as $key => $case) {
  if ($stack4->push($case) == false) {
    throw new Exception('Stack5 push error');
  }
  $stack5->pop();
  if ($stack5->size() !== 0) {
    throw new Exception('Stack5 size error');
  }
}

$test_array6 = [0, 1, 2, null, true, false];
$stack6 = new \Vary\Stack([0, 1, 2, null, true, false]);
foreach($test_array6 as $case) {
  $stack6->pop();
}
if ($stack6->size() !== 0) {
  throw new Exception('Stack6 wrong size');
}