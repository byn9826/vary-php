<?php

class TestHooks extends Test {
  public $marker = 0;

  public function beforeCreate() {
    $this->marker += 1;
  }

  public function afterCreate() {
    $this->marker += 2;
  }

  public function beforeUpdate() {
    $this->marker += 3;
  }

  public function afterUpdate() {
    $this->marker += 4;
  }
}