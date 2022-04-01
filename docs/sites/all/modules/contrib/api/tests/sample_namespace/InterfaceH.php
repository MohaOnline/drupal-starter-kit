<?php

/**
 * @file Contains \api\test1\InterfaceH.
 */

namespace api\test1;

use api\test2\InterfaceC;

/**
 * An interface that inherits two same-named methods.
 */
interface InterfaceH extends InterfaceC, InterfaceD {
}
