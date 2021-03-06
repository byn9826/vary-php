<?php

include_once(__DIR__ . '/models/Test.php');
include_once(__DIR__ . '/models/TestHooks.php');
include_once(__DIR__ . '/models/Dog.php');
include_once(__DIR__ . '/models/Human.php');

$new_test = new Test();
$new_test->name = '123456';
$new_test->note = 'new1';
$new_test->create();
$new_test = new Test();
$new_test->name = '123456';
$new_test->note = 'new1';
$new_test->create();
$new_test = new Test();
$new_test->name = '123456';
$new_test->note = 'new2';
$new_test->create();
$tests = Test::list();
if (
  count($tests) !== 6
  || $tests[3]->name !== '123456'
  || $tests[3]->note !== 'new1'
  || $tests[4]->name !== '123456'
  || $tests[4]->note !== 'new1'
  || $tests[5]->name !== '123456'
  || $tests[5]->note !== 'new2'
) {
  throw new Exception('Model create error');
};
Test::deleting([
  'where' => [
    'name' => '123456',
    'note' => 'new1'
  ]
]);
$tests = Test::list();
if (
  count($tests) !== 4
  || $tests[3]->name !== '123456'
  || $tests[3]->note !== 'new2'
) {
  throw new Exception('Model deleting error');
};
Test::deleting([
  'where' => [
    'name' => '123456'
  ]
]);

$tests = Test::list();
if (
  count($tests) !== 3
  || $tests[0]->name !== 'test'
  || $tests[1]->name !== 'test2'
  || $tests[2]->name !== 'test3'
) {
  throw new Exception('Model create error');
};

Test::updating([
  'set' => [
    'id' => '5',
    'name' => 'ccc',
    'note' => 'ddd',
  ],
  'where' => [
    'id' => 2
  ]
]);
$tests = Test::list();
if ($tests[0]->id !== '1' || $tests[0]->name !== 'test' || $tests[0]->note !== 'test1') {
  throw new Exception('Model updating error');
}
if ($tests[1]->id !== '2' || $tests[1]->name !== 'ccc' || $tests[1]->note !== 'ddd') {
  throw new Exception('Model updating error');
}
if ($tests[2]->id !== '3' || $tests[2]->name !== 'test3' || $tests[2]->note !== 'lalala') {
  throw new Exception('Model updating error');
}
Test::updating([
  'set' => [
    'name' => 'test2',
    'note' => 'haha',
  ],
  'where' => [
    'name' => 'ccc'
  ]
]);
$tests = Test::list();
if ($tests[0]->id !== '1' || $tests[0]->name !== 'test' || $tests[0]->note !== 'test1') {
  throw new Exception('Model updating error');
}
if ($tests[1]->id !== '2' || $tests[1]->name !== 'test2' || $tests[1]->note !== 'haha') {
  throw new Exception('Model updating error');
}
if ($tests[2]->id !== '3' || $tests[2]->name !== 'test3' || $tests[2]->note !== 'lalala') {
  throw new Exception('Model updating error');
}

$new_test = new Test();
$new_test->id = 1;
$new_test->name = 'New Name';
$new_test->note = 'It is a new note';
$new_test->create();
if ($new_test->name !== 'New Name' || $new_test->note !== 'It is a new note' || !isset($new_test->id) || $new_test->id === 1) {
  throw new Exception('Model update error');
}
$tests = Test::list();
if (end($tests)->name !== 'New Name' || end($tests)->note !== 'It is a new note') {
  throw new Exception('Model update error');
}
$new_test = new Test();
$new_test->name = 'Super Name';
$new_test->note = 'It is a super note';
$new_test->create();
$new_test->name = 'Secondary Super Name';
$new_test->update();
$new_test1 = Test::get([
  'where' => ['name' => 'Secondary Super Name']
]);

if ($new_test->id !== $new_test1->id || $new_test1->name !== 'Secondary Super Name' || $new_test1->note !== 'It is a super note') {
  throw new Exception('Model update error');
}

$tests = Test::list();
foreach ($tests as $key => $test) {
  if ($key > 2) {
    $test->delete();
  }
}
$tests = Test::list();
if (count($tests) !== 3 || $tests[0]->id !== '1' || $tests[1]->id !== '2' || $tests[2]->id !== '3') {
  throw new Exception('Model list error');
}
if (Test::$__table__ !== 'test_connection' || \Vary\Model::$__table__ !== null) {
  throw new Exception('Model useTable error');
}
if (Test::$__primary__ !== 'id' || \Vary\Model::$__primary__ !== null) {
  throw new Exception('Model usePrimary error');
}
if (Test::$__columns__[0] !== 'id' || Test::$__columns__[1] !== 'name' || Test::$__columns__[2] !== 'note') {
  throw new Exception('Model useTable error');
}
$tests = Test::list([
  'limit' => 1
]);
if (count($tests) !== 1 || $tests[0]->id !== '1') {
  throw new Exception('Model list error');
}
$tests = Test::list([
  'limit' => 2
]);
if (count($tests) !== 2 || $tests[0]->id !== '1' || $tests[1]->id !== '2') {
  throw new Exception('Model list error');
}
$tests[0]->name = 'new name';
$tests[0]->note = 'new note';
if ($tests[0]->__origin__ !== ['id' => '1', 'name' => 'test', 'note' => 'test1']) {
  throw new Exception('Model list error');
}
if ($tests[1]->__origin__ !== ['id' => '2', 'name' => 'test2', 'note' => 'haha']) {
  throw new Exception('Model list error');
}
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$test_valid = Test::list([
  'limit' => 1
]);
if (
  $test_valid[0]->id !== '1'
  || $test_valid[0]->name !== 'new name'
  || $test_valid[0]->note !== 'new note'
) {
  throw new Exception('_conn list error');
}
$tests[0]->name = 'test';
$tests[0]->note = 'test1';
$tests[0]->test = 'testing';
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$test_valid = Test::list([
  'limit' => 1
]);
if (
  $test_valid[0]->id !== '1'
  || $test_valid[0]->name !== 'test'
  || $test_valid[0]->note !== 'test1'
) {
  throw new Exception('Model update error');
}

$tests = Test::list([
  'limit' => 1,
  'offset' => 0
]);
if (
  $tests[0]->id !== '1'
  || $tests[0]->name !== 'test'
  || $tests[0]->note !== 'test1'
) {
  throw new Exception('Model list error');
}

$tests = Test::list([
  'limit' => 2,
  'offset' => 1
]);
if (
  $tests[0]->id !== '2'
  || $tests[0]->name !== 'test2'
  || $tests[0]->note !== 'haha'
  || $tests[1]->id !== '3'
  || $tests[1]->name !== 'test3'
  || $tests[1]->note !== 'lalala'
) {
  throw new Exception('Model list error');
}
$tests = Test::list([
  'limit' => 1,
  'orderBy' => ['id DESC']
]);
if (
  $tests[0]->id !== '3'
  || $tests[0]->name !== 'test3'
  || $tests[0]->note !== 'lalala'
) {
  throw new Exception('Model orderBy error');
}
$tests[0]->name = 'test2';
$tests[0]->id = '100';
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$tests = Test::list([
  'orderBy' => ['name DESC', 'id ASC']
]);
if (
  $tests[0]->id !== '2'
  || $tests[0]->name !== 'test2'
  || $tests[0]->note !== 'haha'
  || $tests[1]->id !== '3'
  || $tests[1]->name !== 'test2'
  || $tests[1]->note !== 'lalala'
  || $tests[2]->id !== '1'
  || $tests[2]->name !== 'test'
  || $tests[2]->note !== 'test1'
) {
  throw new Exception('Model orderBy error');
}
$tests = Test::list([
  'orderBy' => ['name ASC', 'id DESC']
]);
if (
  $tests[0]->id !== '1'
  || $tests[0]->name !== 'test'
  || $tests[0]->note !== 'test1'
  || $tests[1]->id !== '3'
  || $tests[1]->name !== 'test2'
  || $tests[1]->note !== 'lalala'
  || $tests[2]->id !== '2'
  || $tests[2]->name !== 'test2'
  || $tests[2]->note !== 'haha'
) {
  throw new Exception('Model orderBy error');
}

$tests = Test::list([
  'orderBy' => ['id DESC']
]);
$tests[0]->name = 'test3';
$tests[0]->id = 3000;
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$tests = Test::list([
  'limit' => 1,
  'select' => ['id', 'name']
]);
if (
  count($tests) !== 1
  || $tests[0]->id !== '1'
  || $tests[0]->name !== 'test'
  || isset($tests[0]->note)
) {
  throw new Exception('Model select error');
}

$tests = Test::list([
  'where' => [
    'id' => 1
  ]
]);
if (count($tests) !== 1 || $tests[0]->id !== '1') {
  throw new Exception('Model where error');
}
$tests[0]->name = 'test2';
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}
$tests = Test::list([
  'where' => [
    'name' => 'test2'
  ]
]);
if (count($tests) !== 2 || $tests[0]->id !== '1' || $tests[1]->id !== '2') {
  throw new Exception('Model where error');
}
$tests = Test::list([
  'where' => [
    'id' => 1,
    'name' => 'test2'
  ]
]);
if (count($tests) !== 1 || $tests[0]->id !== '1') {
  throw new Exception('Model where error');
}
$tests[0]->name = 'test';
if ($tests[0]->update() !== true) {
  throw new Exception('Model update error');
}

$test = Test::get(1);
if (
  $test->id !== '1'
  || $test->name !== 'test'
  || $test->note !== 'test1'
) {
  throw new Exception('Model get error');
}
$test = Test::get('2');
if (
  $test->id !== '2'
  || $test->name !== 'test2'
  || $test->note !== 'haha'
) {
  throw new Exception('Model get error');
}

$test = Test::get([
  'orderBy' => ['id DESC'],
]);
if (
  $test->id !== '3'
  || $test->name !== 'test3'
  || $test->note !== 'lalala'
) {
  throw new Exception('Model get error');
}

$test = Test::get([
  'orderBy' => ['name ASC']
]);
if (
  $test->id !== '1'
  || $test->name !== 'test'
  || $test->note !== 'test1'
) {
  throw new Exception('Model get error');
}

$test = Test::get([
  'where' => [
    'id' => 1,
    'name' => 'test2'
  ]
]);
if ($test !== false) {
  throw new Exception('Model get error');
}
$test = Test::get([
  'where' => ['id' => 2],
  'select' => ['name']
]);
if ($test->name !== 'test2' || isset($test->note) || !isset($test->id)) {
  throw new Exception('Model get error');
}

$test_hooks = TestHooks::list();
if (
  count($test_hooks) !== 3
  || $test_hooks[0]->name !== 'test'
  || $test_hooks[1]->name !== 'test2'
  || $test_hooks[2]->name !== 'test3'
) {
  throw new Exception('Model extends error');
};

$new_test = new TestHooks();
if ($new_test->marker !== 0) {
  throw new Exception('Model hooks error');
};
$new_test->name = 'Super Hooks';
$new_test->note = 'It is a super hook';
$new_test->create();
if ($new_test->marker !== 3) {
  throw new Exception('Model hooks error');
};
$new_test->name = 'Super Hooks1';
$new_test->update();
if ($new_test->marker !== 10) {
  throw new Exception('Model hooks error');
};
$new_test->delete();
$test_hooks = TestHooks::list();
if (
  count($test_hooks) !== 3
  || $test_hooks[0]->name !== 'test'
  || $test_hooks[1]->name !== 'test2'
  || $test_hooks[2]->name !== 'test3'
) {
  throw new Exception('Model hooks error');
};

$humans = Human::list([
  'where' => [
    'human_id' => '1'
  ]
]);
if (
  count($humans) !== 1 || $humans[0]->human_name !== 'baozier'
) {
  throw new Exception('Model list error');
};
$humans = Human::list([
  'where' => [
    'human_id' => '2'
  ]
]);
if (
  count($humans) !== 1 || $humans[0]->human_name !== 'mimi'
) {
  throw new Exception('Model list error');
};

$humans = Human::list([
  'where' => [
    'human_id' => '1'
  ],
  'orWhere' => [
    'human_id' => '2'
  ]
]);

if (
  count($humans) !== 2
  || $humans[0]->human_name !== 'baozier'
  || $humans[0]->dog_id !== '1'
  || $humans[1]->human_name !== 'mimi'
  || $humans[1]->dog_id !== '1'
) {
  throw new Exception('Model orWhere error');
};

Human::updating([
  'set' => [
    'dog_id' => '2'
  ],
  'where' => [
    'human_id' => '1'
  ],
  'orWhere' => [
    'human_id' => '2'
  ]
]);

$human = Human::get([
  'where' => ['dog_id' => '2']
]);
if (
  $human->human_name !== 'baozier'
  || $human->dog_id !== '2'
) {
  throw new Exception('Model orWhere error');
};

$human = Human::get([
  'where' => ['dog_id' => '2'],
  'orderBy' => ['human_id DESC']
]);
if (
  $human->human_name !== 'mimi'
  || $human->dog_id !== '2'
) {
  throw new Exception('Model orWhere error');
};

Human::updating([
  'set' => ['dog_id' => '1']
]);
$humans = Human::list();
if (
  count($humans) !== 2
  || $humans[0]->human_name !== 'baozier'
  || $humans[0]->dog_id !== '1'
  || $humans[1]->human_name !== 'mimi'
  || $humans[1]->dog_id !== '1'
) {
  throw new Exception('Model orWhere error');
};

$human = new Human();
$human->human_name = 'H1';
$human->create();
$human = new Human();
$human->human_name = 'H2';
$human->create();
$humans = Human::list();
if (count($humans) !== 4) {
  throw new Exception('Model create error');
};
Human::deleting([
  'where' => [
    'human_name' => 'H1'
  ],
  'orWhere' => [
    'human_name' => 'H2'
  ]
]);
$humans = Human::list();
if (
  count($humans) !== 2
  || $humans[0]->human_name !== 'baozier'
  || $humans[1]->human_name !== 'mimi'
) {
  throw new Exception('Model orWhere error');
};