Session Proxy
=============

Session proxy is not a module: it is a replacement for Drupal core session
handling API.

It acts as a proxy between fixed core procedural API towards a fully object
oriented, more flexible and easier to extend session management API.

Basically, it strongly encapsulate the original API, separating user management
against raw session storage: by using it you are able to implement any session
storage engine (memcache, redis, ... ) without handling cookies and by users
yourself.

Provided session engines
------------------------

It ships three different session managing implementations:

 1. Database session handling: core default, ported to the object oriented
    API. It comes with an additional SQL query done more than core session.inc
    default implementation due to the API strong encapsulation.

 2. Native PHP session handling: by selecting this session management mode,
    you explicitely remove any core session management, and let PHP manage
    it. This can be useful if you intend to use low-level PHP extensions,
    such as Memcache or PhpRedis to handle session in PHP low-level layer
    instead of core. This allow to use any PHP native session storage to
    work gracefully: some of them are high performance.

 3. Cache based storage engine: Using the same session handling than core,
    but deporting raw session storage into a cache backend instance. You can
    use any already-functionning cache backend to store sessions. Sessions
    will use their own bin, and won't be cleared at cache clear all time.

Installation and configuration
------------------------------

If you intend to use the Drupal default cache implementation (database cache)
for session handling, you need to enable and install the "session_proxy" module
to ensure the database table (bin) for the sessions storage cache backend will
be created. For all other cases, this is unnecessary. 

All configuration will reside into your settings.php file, see the documented
example below for details:

// -- Database backend, session management by Drupal ---------
//
//  This is the classical way to handle sessions in Drupal
// Actually no session locking applied in Drupal so race conditions on session
// write operations (for the same user/session) can happen.
    // Replace core session management with Session Proxy API.
$conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';
    //
    // If you set this to TRUE, session management will be PHP native session
    // management and not Drupal session management (which is database sessions
    // in classical Drupal installations).
    // By doing this, all other parameters below will be ignored.
    // by using native session management you'll reactivate the PHP native
    // session locking management, that means session_start() can only be done 
    // for one HTTP request in the same time (no parallel load of images for
    // imagecache images, no parallel browser tabs, auto-serialization of 
    // requests which belongs to the same session)
$conf['session_storage_force_default'] = FALSE;

    // PHP class to use as session storage engine. Default is the database
    // implementation (port of the actual core session management). By setting
    // this parameter, all others settings except 'session_storage_force_default'
    // or 'session_storage_options' will be ignored.
$conf['session_storage_class'] = 'SessionProxy_Storage_Database';

// -- APC backend & session managment by drupal ---------
//
// (use it only if you have only one server)
// WARNING: be careful with APC backend for sessions, when the cache is full
// it perform an automatic cache clear, same thing with apache reload
// and that mean you may loose your sessions, so use with extreme care
// Actually no session locking applied in Drupal so race conditions on session
// write operations (for the same user/session) can happen (like in default 
// Drupal sessions).
    // Replace core session management with Session Proxy API.
//$conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';
    //
    // do not forget to empty the sessions table in Drupal, not used anymore
    //
    // If you set this to TRUE, session management will be PHP native session
    // management. By doing this, all other parameters below will be ignored.
$conf['session_storage_force_default'] = FALSE;

    // PHP class to use as session storage engine.
    // we use the 'Cache' storage backend to use Drupal7 cache backends
    // (or the same with Drupal6 & cache_backport module)'
$conf['session_storage_class'] = 'SessionProxy_Storage_Cache';
    // Everything into 'session_storage_options' are arbitrary key value pairs,
    // each storage backend will define its own keys.
    // For cache backend, the only mandatory one is the class name that to use
    // as cache backend. This class must implement DrupalCacheInterface. If you
    // do not set this class 'DrupalDatabaseCache' will be used. 
$conf['session_storage_options']['cache_backend'] = 'DrupalAPCCache';
// if you do not have already configured APC as a cache backend do it there
//$conf['cache_backends'][] = 'sites/all/modules/apc/drupal_apc_cache.inc';
//$conf['apc_show_debug'] = FALSE;

// -- Memcache backend & session managment by drupal --
//
// certainly a very fast thing, 
// Actually no session locking applied in Drupal so race conditions on session
// write operations (for the same user/session) can happen (like in default 
// Drupal sessions).
    // Replace core session management with Session Proxy API.
//$conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';
    //
    // do not forget to empty the sessions table in Drupal, not used anymore
    //
    // If you set this to TRUE, session management will be PHP native session
    // management. By doing this, all other parameters below will be ignored.
$conf['session_storage_force_default'] = FALSE;

    // PHP class to use as session storage engine.
    // we use the 'Cache' storage backend to use Drupal7 cache backends
    // (or the same with Drupal6 & cache_backport module)'
$conf['session_storage_class'] = 'SessionProxy_Storage_Cache';
    // Everything into 'session_storage_options' are arbitrary key value pairs,
    // each storage backend will define its own keys.
    // For cache backend, the only mandatory one is the class name that to use
    // as cache backend. This class must implement DrupalCacheInterface. If you
    // do not set this class 'DrupalDatabaseCache' will be used. 
$conf['session_storage_options']['cache_backend'] = 'MemCacheDrupal';
    // if you do not have already configured APC as a cache backend do it there
    // $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
    // see configuration of 'MemCacheDrupal' for details 
    // (in cache_backport module's README.txt or in the module README.txt)

// ---- Memcache session managment by PHP ----
//
// Here the session proxy module does a very minimum amount of things
// all is done by your PHP configuration
// If you want session locking mechanism you'll need a version > 3.0.4 of the
// memcache-php library. Without that some race conditions on parallel requests
// (ajax, and such) may happen. With it you'll serialize same session executions
// and may experiment some performance loss.
// to get a version > 3 while it's unstable do a "pecl install memcache-beta "
//php-memcache ----
// INI settings
//session.save_handler = memcache
//session.save_path = "tcp://127.0.0.1:11211" 
//memcache.lock_timeout = 15 // memcache-php > 3.0.4, else no locks for sessions
//php-memcached (untested) ---
// INI settings
//session.save_handler = memcached
//session.save_path = "127.0.0.1:11211"
// and this for php-memcached would stay here and not in ini settings
//$conf['memcache_options'][Memcached::MEMC_SESS_LOCK_ATTEMPTS]=30;
//$conf['memcache_options'][Memcached::MEMC_SESS_LOCK_WAIT]=100000;
//$conf['memcache_options'][Memcached::MEMC_SESS_LOCK_EXPIRATION]=30;
    // Replace core session management with Session Proxy API.
$conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';
    //
    // do not forget to empty the sessions table in Drupal, not used anymore
    //
    // If you set this to TRUE, session management will be PHP native session
    // management. By doing this, all other parameters below will be ignored.
$conf['session_storage_force_default'] = TRUE;

// ---- File cache Backend, session managment by Drupal ----
// back to files like in native PHP implementation, but without session locking
// and with a write delay, so may be quite really faster
// Actually no session locking applied in Drupal so race conditions on session
// write operations (for the same user/session) can happen (like in default 
// Drupal sessions).
    // Replace core session management with Session Proxy API.
$conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';
    //
    // do not forget to empty the sessions table in Drupal, not used anymore
    //
    // If you set this to TRUE, session management will be PHP native session
    // management. By doing this, all other parameters below will be ignored.
$conf['session_storage_force_default'] = FALSE;

    // PHP class to use as session storage engine.
    // we use the 'Cache' storage backend to use Drupal7 cache backends
    // (or the same with Drupal6 & cache_backport module)'
$conf['session_storage_class'] = 'SessionProxy_Storage_Cache';
    // Everything into 'session_storage_options' are arbitrary key value pairs,
    // each storage backend will define its own keys.
    // For cache backend, the only mandatory one is the class name that to use
    // as cache backend. This class must implement DrupalCacheInterface. If you
    // do not set this class 'DrupalDatabaseCache' will be used. 
$conf['session_storage_options']['cache_backend'] = 'DrupalFileCache';
    // if you do not have already configured APC as a cache backend do it there
    // $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
    // see configuration of 'MemCacheDrupal' for details 
    // (in cache_backport module's README.txt or in the module README.txt)
    // $conf['cache_backends'][] = 'sites/all/modules/filecache/filecache.inc';
    // $conf['filecache_fast_pagecache'] = TRUE; // set TRUE to enable fast page serving
    // $conf['filecache_directory'] = $conf['file_directory_temp'] . '/filecache';

// ---- Redis cache Backend, session managment by Drupal ----
//
// Here is an example of usage the cache storage engine, using the Redis
// module for storing sessions.
// Actually no session locking applied in Drupal so race conditions on session
// write operations (for the same user/session) can happen (like in default 
// Drupal sessions).
    // Replace core session management with Session Proxy API.
$conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';
    //
    // do not forget to empty the sessions table in Drupal, not used anymore
    //
    // If you set this to TRUE, session management will be PHP native session
    // management. By doing this, all other parameters below will be ignored.
$conf['session_storage_force_default'] = FALSE;

    // PHP class to use as session storage engine.
    // we use the 'Cache' storage backend to use Drupal7 cache backends
    // (or the same with Drupal6 & cache_backport module)'
$conf['session_storage_class'] = 'SessionProxy_Storage_Cache';
    // Everything into 'session_storage_options' are arbitrary key value pairs,
    // each storage backend will define its own keys.
    // For cache backend, the only mandatory one is the class name that to use
    // as cache backend. This class must implement DrupalCacheInterface. If you
    // do not set this class 'DrupalDatabaseCache' will be used. 
$conf['session_storage_options']['cache_backend'] = 'Redis_Cache';
    // Tell Drupal to load the Redis backend properly (if not done previously in
    // settings), see the Redis module documentation for details about this.
    //$conf['cache_backends'][] = 'sites/all/modules/redis/redis.autoload.inc';
    //$conf['redis_client_interface'] = 'PhpRedis';

Session Locks
--------------
Native PHP file implementation of sessions prevents race conditions with
parallel requests from the same session by locking on session_start(), so every 
parallel session gets serialized.

Drupal 7 implementation of the session in the database allows parallel
executions with the caveat of overriding the session content with the content
of the last-to-end parallel query. Drupal also reduces the number of write
requests by delaying the session write at the end of the request.

Actually all sessions managed by Drupal or this session proxy module can have
the same race condition as the default Drupal installation. But with this module
you can use 'session_storage_force_default' setting to get the native PHP
implementation or the memcache-native implementation of the sessions 
(http://php.net/manual/en/memcached.sessions.php) and have the session locks.

Notes
-----

If you download and properly install the Autoloader Early module, you may
experience better autoloading performances. You can download it at:

  http://drupal.org/project/autoloaderearly

Some cache backend links:

  * Redis - http://drupal.org/project/redis

  * Memcache API and Integration - http://drupal.org/project/memcache
  
Cache Backport : 

If you want to use the (near to come) Drupal6 version of this module you will
need the Cache Backport module and the Drupal7 versions of the backends.
  * Cache Backport : http://drupal.org/project/cache_backport
