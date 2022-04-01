<?php

/**
 * @file
 * Class inheritance loop.
 */

class A extends B {
}

class B extends C {
}

class C extends A {
}
