<?php

$test = new \Vary\OrderedList();
foreach([1, 0, 3, -1, 50] as $value) {
  $test->add($value);
}
if ($test->value() !== [-1, 0, 1, 3, 50]) {
  throw new Exception('OrderedList construct error');
}
if ($test->size() !== 5 || $test->value()[0] !== -1 || $test->value()[4] !== 50) {
  throw new Exception('OrderedList add error');
}
if ($test->shift() !== -1) {
  throw new Exception('OrderedList shift error');
}
if ($test->shift() !== 0) {
  throw new Exception('OrderedList shift error');
}
if ($test->pop() !== 50) {
  throw new Exception('OrderedList pop error');
}
if ($test->pop() !== 3) {
  throw new Exception('OrderedList pop error');
}
if ($test->size() !== 1 || $test->value()[0] !== 1) {
  throw new Exception('OrderedList size error');
}
if ($test->add(0) !== 0) {
  throw new Exception('OrderedList add error');
}
if ($test->add(2) !== 2) {
  throw new Exception('OrderedList add error');
}
if ($test->add(1) !== false) {
  throw new Exception('OrderedList add error');
}
if ($test->removeIndex(0) !== 0) {
  throw new Exception('OrderedList removeIndex error');
}
if ($test->removeIndex(3) !== null) {
  throw new Exception('OrderedList removeIndex error');
}
if ($test->remove(0) !== false) {
  throw new Exception('OrderedList remove error');
}
if ($test->remove(2) !== 1) {
  throw new Exception('OrderedList remove error');
}
if ($test->indexOf(1) !== 0) {
  throw new Exception('OrderedList indexOf error');
}
if ($test->indexOf(2) !== false) {
  throw new Exception('OrderedList indexOf error');
}
if ($test->add(2) !== 1) {
  throw new Exception('OrderedList add error');
}
if ($test->indexOf(2) !== 1) {
  throw new Exception('OrderedList indexOf error');
}
$test = new \Vary\OrderedList([1, 0, 3, -1, 50]);
if ($test->value() !== [-1, 0, 1, 3, 50]) {
  throw new Exception('OrderedList construct error');
}

$test1 = new \Vary\OrderedList();
$add1 = $test1->add(1);
if ($add1 !== 0 || $test1->size() !== 1) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(3);
if ($add1 !== 1 || $test1->value()[0] !== 1 || $test1->value()[1] !== 3) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(2);
if ($add1 !== 1 || $test1->value()[0] !== 1 || $test1->value()[1] !== 2 || $test1->value()[2] !== 3) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(-1);
if ($add1 !== 0 || $test1->value()[0] !== -1) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(-1);
if ($add1 !== false) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(4);
if ($add1 !== 4 && $test1->value()[4] !== 4) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(100);
if ($add1 !== 5 && $test1->value()[5] !== 100) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(0);
if ($add1 !== 1 && $test1->value()[1] !== 0) {
  throw new Exception('OrderedList1 add error');
}
$remove1 = $test1->shift();
if ($remove1 !== -1) {
  throw new Exception('OrderedList1 shift error');
}
$remove1 = $test1->shift();
if ($remove1 !== 0) {
  throw new Exception('OrderedList1 shift error');
}
$remove1 = $test1->pop();
if ($remove1 !== 100) {
  throw new Exception('OrderedList1 pop error');
}
$remove1 = $test1->pop();
if ($remove1 !== 4) {
  throw new Exception('OrderedList1 pop error');
}
if ($test1->value()[0] !== 1 || $test1->value()[1] !== 2 || $test1->value()[2] !== 3) {
  throw new Exception('OrderedList1 remove error');
}
$remove_index = $test1->removeIndex(0);
if ($remove_index !== 1) {
  throw new Exception('OrderedList1 remove index error');
}
$remove_index = $test1->removeIndex(1);
if ($remove_index !== 3) {
  throw new Exception('OrderedList1 remove index error');
}
$remove_index = $test1->removeIndex(1);
if ($remove_index !== null) {
  throw new Exception('OrderedList1 remove index error');
}
if ($test1->size() !== 1 || $test1->value()[0] !== 2) {
  throw new Exception('OrderedList1 remove index error');
}
$remove = $test1->remove(1);
if ($remove !== false) {
  throw new Exception('OrderedList1 remove error');
}
$remove = $test1->remove(2);
if ($remove !== 0 || $test1->size() !== 0) {
  throw new Exception('OrderedList1 remove error');
}
