<?php
$test_array1 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Queue(), [1, [new \Vary\Queue()]] 
];
$queue1 = new \Vary\Queue();
if ($queue1->size() !== 0) {
  throw new Exception('Queue1 initial error');
}
foreach($test_array1 as $case) {
  if ($queue1->enqueue($case) == false) {
    throw new Exception('Queue1 1 enqueue error');
  }
}
if ($queue1->size() !== 12) {
  throw new Exception('Queue1 size error');
}

$test_array2 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Queue(), [1, [new \Vary\Queue()]] 
];
$queue2 = new \Vary\Queue();
if ($queue2->size() !== 0) {
  throw new Exception('Queue2 initial error');
}
foreach($test_array2 as $key => $case) {
  if ($queue2->enqueue($case) == false) {
    throw new Exception('Queue2 push error');
  }
  if ($queue2->size() - 1 !== $key) {
    throw new Exception('Queue2 size error');
  }
}
if ($queue2->size() !== 12) {
  throw new Exception('Queue2 final error');
}

$queue3 = new \Vary\Queue([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Queue(), [1, [new \Vary\Queue()]] 
]);
if ($queue3->size() !== count($queue3->_items)) {
  throw new Exception('Queue3 initial error');
}
if ($queue3->size() !== 12) {
  throw new Exception('Queue3 final error');
}

$test_array4 = [0, 1, 2, null, true, false];
$queue4 = new \Vary\Queue([0, 1, 2, null, true, false]);
if ($queue4->size() !== 6) {
  throw new Exception('Queue4 initial error');
}
foreach($test_array4 as $key => $case) {
  $return = $queue4->dequeue();
  if ($return !== $test_array4[$key]) {
    throw new Exception('Queue4 dequeue error');
  }
}

$test_array5 = [0, 1, 2, null, true, false];
$queue5 = new \Vary\Queue();
foreach($test_array5 as $key => $case) {
  if ($queue5->enqueue($case) == false) {
    throw new Exception('Queue5 enqueue error');
  }
  $queue5->dequeue();
  if ($queue5->size() !== 0) {
    throw new Exception('Queue5 size error');
  }
}

$test_array6 = [0, 1, 2, null, true, false];
$queue6 = new \Vary\Queue([0, 1, 2, null, true, false]);
foreach($test_array6 as $case) {
  $queue6->dequeue();
}
if ($queue6->size() !== 0) {
  throw new Exception('Queue5 wrong size');
}
