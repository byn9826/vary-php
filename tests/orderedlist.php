<?php
$test1 = new \Vary\OrderedList();
$add1 = $test1->add(1);
if ($add1 !== 0 || $test1->size() !== 1) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(3);
if ($add1 !== 1 || $test1->_items[0] !== 1 || $test1->_items[1] !== 3) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(2);
if ($add1 !== 1 || $test1->_items[0] !== 1 || $test1->_items[1] !== 2 || $test1->_items[2] !== 3) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(-1);
if ($add1 !== 0 || $test1->_items[0] !== -1) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(-1);
if ($add1 !== false) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(4);
if ($add1 !== 4 && $test1->_items[4] !== 4) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(100);
if ($add1 !== 5 && $test1->_items[5] !== 100) {
  throw new Exception('OrderedList1 add error');
}
$add1 = $test1->add(0);
if ($add1 !== 1 && $test1->_items[1] !== 0) {
  throw new Exception('OrderedList1 add error');
}
$remove1 = $test1->removeFront();
if ($remove1 !== -1) {
  throw new Exception('OrderedList1 removeFront error');
}
$remove1 = $test1->removeFront();
if ($remove1 !== 0) {
  throw new Exception('OrderedList1 removeFront error');
}
$remove1 = $test1->removeRear();
if ($remove1 !== 100) {
  throw new Exception('OrderedList1 removeRear error');
}
$remove1 = $test1->removeRear();
if ($remove1 !== 4) {
  throw new Exception('OrderedList1 removeRear error');
}
if ($test1->_items[0] !== 1 || $test1->_items[1] !== 2 || $test1->_items[2] !== 3) {
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
if ($test1->size() !== 1 || $test1->_items[0] !== 2) {
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

$test2 = new \Vary\OrderedList();
foreach([1, 0, 3, -1, 50] as $value) {
  $test2->add($value);
}
if ($test2->size() !== 5 || $test2->_items[0] !== -1 || $test2->_items[4] !== 50) {
  throw new Exception('OrderedList2 add error');
}
if ($test2->removeFront() !== -1) {
  throw new Exception('OrderedList2 removeFront error');
}
if ($test2->removeFront() !== 0) {
  throw new Exception('OrderedList2 removeFront error');
}
if ($test2->removeRear() !== 50) {
  throw new Exception('OrderedList2 removeRear error');
}
if ($test2->removeRear() !== 3) {
  throw new Exception('OrderedList2 removeRear error');
}
if ($test2->size() !== 1 || $test2->_items[0] !== 1) {
  throw new Exception('OrderedList2 size error');
}
if ($test2->add(0) !== 0) {
  throw new Exception('OrderedList2 add error');
}
if ($test2->add(2) !== 2) {
  throw new Exception('OrderedList2 add error');
}
if ($test2->add(1) !== false) {
  throw new Exception('OrderedList2 add error');
}
if ($test2->removeIndex(0) !== 0) {
  throw new Exception('OrderedList2 removeIndex error');
}
if ($test2->removeIndex(3) !== null) {
  throw new Exception('OrderedList2 removeIndex error');
}
if ($test2->remove(0) !== false) {
  throw new Exception('OrderedList2 remove error');
}
if ($test2->remove(2) !== 1) {
  throw new Exception('OrderedList2 remove error');
}
if ($test2->indexOf(1) !== 0) {
  throw new Exception('OrderedList2 indexOf error');
}
if ($test2->indexOf(2) !== false) {
  throw new Exception('OrderedList2 indexOf error');
}
if ($test2->add(2) !== 1) {
  throw new Exception('OrderedList2 add error');
}
if ($test2->indexOf(2) !== 1) {
  throw new Exception('OrderedList2 indexOf error');
}