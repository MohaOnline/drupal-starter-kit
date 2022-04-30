<?php

// @codingStandardsIgnoreFile
/*
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This software consists of voluntary contributions made by many individuals
 * and is licensed under the MIT license. For more information, see
 * <http://www.doctrine-project.org>.
 */

namespace OpenlayersDoctrine\Common\Reflection;

/**
 * In PHP 7.2 the signature of \ReflectionClass::newInstance() has changed.
 *
 * Unfortunately a fix cannot be made that is compatible with PHP5.5 and other
 * versions of PHP we currently support. If Doctrine\Common is upgraded to a
 * version which has the fix then we will need to change this class to work on
 * PHP5.5.
 *
 * @see https://github.com/doctrine/common/pull/822
 * @see https://github.com/php/php-src/pull/2893
 */
if (version_compare(phpversion(), '7', '<')) {
  return;
}

use ReflectionClass;
use ReflectionException;

class StaticReflectionClass extends ReflectionClass
{
    /**
     * The static reflection parser object.
     *
     * @var StaticReflectionParser
     */
    private $staticReflectionParser;

    /**
     * @param StaticReflectionParser $staticReflectionParser
     */
    public function __construct(StaticReflectionParser $staticReflectionParser)
    {
        $this->staticReflectionParser = $staticReflectionParser;
    }

    /**
     * {@inheritDoc}
     */
    public function getName()
    {
        return (string) $this->staticReflectionParser->getClassName();
    }

    /**
     * {@inheritDoc}
     */
    public function getDocComment()
    {
        return (string) $this->staticReflectionParser->getDocComment();
    }

}
