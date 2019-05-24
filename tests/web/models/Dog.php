<?php

class Dog extends \Vary\Model {
  public static function config()
  {
    self::useTable('dog');
    self::usePrimary('id');
  }
}