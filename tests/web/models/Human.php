<?php

class Human extends \Vary\Model {
  public static function config()
  {
    self::useTable('human');
    self::usePrimary('id');
  }
}