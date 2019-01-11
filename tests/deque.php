<?php
$test_array1 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Stack(), [1, [new \Vary\Stack()]] 
];
$deque1 = new \Vary\Deque();
if ($deque1->size() !== 0) {
  throw new Exception('Deque1 initial error');
}
foreach($test_array1 as $case) {
  if ($deque1->addRear($case) == false) {
    throw new Exception('Deque1 addRear error');
  }
}
if ($deque1->size() !== 12) {
  throw new Exception('Deque1 size error');
}

$test_array2 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Stack(), [1, [new \Vary\Stack()]] 
];
$deque2 = new \Vary\Deque();
if ($deque2->size() !== 0) {
  throw new Exception('Deque2 initial error');
}
foreach($test_array2 as $key => $case) {
  if ($deque2->addRear($case) == false) {
    throw new Exception('Deque2 addRear error');
  }
  if ($deque2->size() - 1 !== $key) {
    throw new Exception('Deque2 size error');
  }
}
if ($deque2->size() !== 12) {
  throw new Exception('Deque2 final error');
}

$deque3 = new \Vary\Deque([
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Deque(), [1, [new \Vary\Deque()]] 
]);
if ($deque3->size() !== count($deque3->_items)) {
  throw new Exception('Deque3 initial error');
}
if ($deque3->size() !== 12) {
  throw new Exception('Deque3 final error');
}

$test_array4 = [0, 1, 2, null, true, false];
$deque4 = new \Vary\Deque([0, 1, 2, null, true, false]);
if ($deque4->size() !== 6) {
  throw new Exception('Deque4 initial error');
}
foreach($test_array4 as $key => $case) {
  $return = $deque4->removeRear();
  if ($return !== $test_array4[count($test_array4) - 1 - $key]) {
    throw new Exception('Deque4 removeRear error');
  }
}

$test_array5 = [0, 1, 2, null, true, false];
$deque5 = new \Vary\Deque();
foreach($test_array5 as $key => $case) {
  if ($deque5->addRear($case) == false) {
    throw new Exception('Deque5 addRear error');
  }
  $deque5->removeRear();
  if ($deque5->size() !== 0) {
    throw new Exception('Deque5 size error');
  }
}

$test_array6 = [0, 1, 2, null, true, false];
$deque6 = new \Vary\Deque([0, 1, 2, null, true, false]);
foreach($test_array6 as $case) {
  $deque6->removeRear();
}
if ($deque6->size() !== 0) {
  throw new Exception('Deque6 wrong size');
}


$test_array7 = [0, 1, 2, null, true, false];
$deque7 = new \Vary\Deque([0, 1, 2, null, true, false]);
if ($deque7->size() !== 6) {
  throw new Exception('Deque7 initial error');
}
foreach($test_array7 as $key => $case) {
  $return = $deque7->removeFront();
  if ($return !== $test_array7[$key]) {
    throw new Exception('Deque7 removeFront error');
  }
}

$test_array8 = [0, 1, 2, null, true, false];
$deque8 = new \Vary\Deque();
foreach($test_array8 as $key => $case) {
  if ($deque8->addRear($case) == false) {
    throw new Exception('Deque8 addRear error');
  }
  $deque8->removeFront();
  if ($deque8->size() !== 0) {
    throw new Exception('Deque8 size error');
  }
}

$test_array9 = [0, 1, 2, null, true, false];
$deque9 = new \Vary\Deque([0, 1, 2, null, true, false]);
foreach($test_array9 as $case) {
  $deque9->removeFront();
}
if ($deque9->size() !== 0) {
  throw new Exception('Deque9 wrong size');
}

$test_array10 = [
  0, 1, 2, null, true, false, [], [1], [1, '1', false], 
  [1, ['1']], new \Vary\Stack(), [1, [new \Vary\Stack()]] 
];
$deque10 = new \Vary\Deque();
if ($deque10->size() !== 0) {
  throw new Exception('Deque10 initial error');
}
foreach($test_array10 as $key => $case) {
  if ($deque10->addFront($case) == false) {
    throw new Exception('Deque10 addFront error');
  }
  if ($deque10->size() - 1 !== $key) {
    throw new Exception('Deque10 size error');
  }
}
if ($deque10->size() !== 12) {
  throw new Exception('Deque10 final error');
}

$test_array11 = [0, 1, 2, null, true, false];
$deque11 = new \Vary\Deque();
foreach($test_array11 as $key => $case) {
  if ($deque11->addFront($case) == false) {
    throw new Exception('Deque11 addFront error');
  }
  $return = $deque11->removeFront();
  if ($return !== $test_array11[$key]) {
    throw new Exception('Deque11 removeFront error');
  }
  if ($deque11->size() !== 0) {
    throw new Exception('Deque11 size error');
  }
}

$test_array12 = [0, 1, 2, null, true, false];
$deque12 = new \Vary\Deque();
foreach($test_array12 as $key => $case) {
  if ($deque12->addFront($case) == false) {
    throw new Exception('Deque12 addFront error');
  }
}
foreach($test_array12 as $key => $case) {
  $return = $deque12->removeFront();
  if ($return !== $test_array12[count($test_array12) - 1 - $key]) {
    throw new Exception('Deque12 removeFront error');
  }
}
if ($deque12->size() !== 0) {
  throw new Exception('Deque12 size error');
}

$test_array13 = [0, 1, 2, null];
$deque13 = new \Vary\Deque();
foreach($test_array13 as $key => $case) {
  if ($key === 0 || $key === 2) {
    if ($deque13->addFront($case) == false) {
      throw new Exception('Deque13 addFront error');
    }
  } else {
    if ($deque13->addRear($case) == false) {
      throw new Exception('Deque13 addFront error');
    }
  }
}
$return = $deque13->removeRear();
if ($return !== null) {
  throw new Exception('Deque13 removeRear error');
}
$return = $deque13->removeFront();
if ($return !== 2) {
  throw new Exception('Deque13 removeRear error');
}
$return = $deque13->removeRear();
if ($return !== 1) {
  throw new Exception('Deque13 removeRear error');
}
$return = $deque13->removeFront();
if ($return !== 0) {
  throw new Exception('Deque13 removeRear error');
}