<?php
/**
 * @file
 * 
 * elFinder conenctor class 
 */

class elFinderDrupal extends elFinder {
	public function __construct($opts) {
        if (is_callable(array($this, 'version'))) {
            
            $ver = floatval($this->version());
            
            if (isset(elFinder::$ApiVersion)) {
                $ver = elFinder::$ApiVersion;
            }
            
            if ($ver == 2.1 && is_callable(array($this, 'revision')) && $this->revision() < 37) {
                $this->connector_unsupported_error();
            }

            parent::__construct($opts);
            
            $this->commands['desc'] = array('target' => TRUE, 'content' => FALSE);
            $this->commands['owner'] = array('target' => TRUE, 'content' => FALSE);
            $this->commands['downloadcount'] = array('target' => TRUE);
            
        } else {
            $this->connector_unsupported_error();
        }
        
	}


  public function connector_unsupported_error() {
      $this->connector_error(t('Unsupported elFinder library version. Please upgrade.'));   
  }
    
  public function connector_error($message) {
    exit(drupal_json_encode(array('error' => array(strip_tags($message)))));   
  }

  /* Overriding search query argument name 'q' since it's already used in Drupal */

  public function commandArgsList($cmd) {
    $this->commands['search']['elfinder_search_q'] = TRUE;
    return $this->commandExists($cmd) ? $this->commands[$cmd] : array();
  }

  protected function search($args) {
    $q = trim($args['elfinder_search_q']);
    $mimes = !empty($args['mimes']) && is_array($args['mimes']) ? $args['mimes'] : array();
    $target = !empty($args['target']) ? $args['target'] : null;
    $result = array();
    $errors = array();

    if ($target) {
      if ($volume = $this->volume($target)) {
        $result = $volume->search($q, $mimes, $target);
        $errors = array_merge($errors, $volume->error());
      }
    } else {
    foreach ($this->volumes as $volume) {
      $result = array_merge($result, $volume->search($q, $mimes));
        $errors = array_merge($errors, $volume->error());
    }
    }

    // Exclude hidden folders from search results.
    $hidden_folders = variable_get('elfinder_settings_misc_hidden_folders', '');
    if($hidden_folders) {
      $hidden_folders = explode(',', $hidden_folders);
      foreach($result AS $key => $file) {
        $parts = explode('/', $file['path']);
        if(in_array($parts[1], $hidden_folders)) {
          unset($result[$key]);
        }
      }
    }

    $result = array('files' => $result);
    if ($errors) {
      $result['warning'] = $errors;
    }
    return $result;
  }

  protected function desc($args) {
    $target = $args['target'];
    $desc = $args['content'];
    $error = array(self::ERROR_UNKNOWN, '#' . $target);

    if (($volume = $this->volume($target)) == FALSE
            || ($file = $volume->file($target)) == FALSE) {
      return array('error' => $this->error($error, self::ERROR_FILE_NOT_FOUND));
    }

    $error[1] = $file['name'];
    
 		if ($volume->driverId() == 'f') {
			return array('desc' => '');
		}

    if ($volume->commandDisabled('desc')) {
      return array('error' => $this->error($error, self::ERROR_ACCESS_DENIED));
    }

    if (($desc = $volume->desc($target, $desc)) == -1) {
      return array('error' => $this->error($error, $volume->error()));
    }

    return array('desc' => $desc);
  }

  protected function owner($args) {
    $target = $args['target'];

    $error = array(self::ERROR_UNKNOWN, '#' . $target);

    if (($volume = $this->volume($target)) == FALSE
            || ($file = $volume->file($target)) == FALSE) {
      return array('error' => $this->error($error, self::ERROR_FILE_NOT_FOUND));
    }

    $error[1] = $file['name'];
    
 		if ($volume->driverId() == 'f') {
			return array('owner' => '');
		}

    if ($volume->commandDisabled('owner')) {
      return array('error' => $this->error($error, self::ERROR_ACCESS_DENIED));
    }

    if (($owner = $volume->owner($target)) == FALSE) {
      return array('error' => $this->error($error, $volume->error()));
    }

    return array('owner' => $owner);
  }

  protected function downloadcount($args) {
    $target = $args['target'];

    $error = array(self::ERROR_UNKNOWN, '#' . $target);

    if (($volume = $this->volume($target)) == FALSE
            || ($file = $volume->file($target)) == FALSE) {
      return array('error' => $this->error($error, self::ERROR_FILE_NOT_FOUND));
    }

    $error[1] = $file['name'];
    
 		if ($volume->driverId() == 'f') {
			return array('downloadcount' => '');
		}

    if ($volume->commandDisabled('downloadcount')) {
      return array('error' => $this->error($error, self::ERROR_ACCESS_DENIED));
    }

    if (($downloadcount = $volume->downloadcount($target)) == FALSE) {
      return array('error' => $this->error($error, $volume->error()));
    }

    return array('downloadcount' => $downloadcount);
  }

  /**
   * Required to output file in browser when volume URL is not set 
   * Return array contains opened file pointer, root itself and required headers
   *
   * @param  array  command arguments
   * @return array
   * @author Dmitry (dio) Levashov
   * */
  protected function file($args) {
    $target = $args['target'];
    $download = !empty($args['download']);
    $h403 = 'HTTP/1.x 403 Access Denied';
    $h404 = 'HTTP/1.x 404 Not Found';

    if (($volume = $this->volume($target)) == FALSE) {
      return array('error' => self::$errors[self::ERROR_FILE_NOT_FOUND], 'header' => $h404, 'raw' => TRUE);
    }

    if (($file = $volume->file($target)) == FALSE) {
      return array('error' => self::$errors[self::ERROR_FILE_NOT_FOUND], 'header' => $h404, 'raw' => TRUE);
    }

    if (!$file['read']) {
      return array('error' => self::$errors[self::ERROR_ACCESS_DENIED], 'header' => $h403, 'raw' => TRUE);
    }

    if ($volume->driverId() != 'f' && (($fp = $volume->open($target)) == FALSE)) {
      return array('error' => self::$errors[self::ERROR_FILE_NOT_FOUND], 'header' => $h404, 'raw' => TRUE);
    }

    $mime = ($download) ? 'application/octet-stream' : $file['mime'];

    $result = array(
        'volume' => $volume,
        'pointer' => $fp,
        'info' => $file,
        'header' => array(
            "Content-Type: " . $mime,
            "Content-Disposition: " . $this->GetContentDisposition($file['name'], $mime, $download),
            "Content-Location: " . $file['name'],
            'Content-Transfer-Encoding: binary',
            "Content-Length: " . $file['size'],
            "Connection: close"
        )
    );
    
    $real_path = $this->realpath($target);
    module_invoke_all('file_download', $volume->drupalpathtouri($real_path));
    
    return $result;
  }

  /**
   * Generating Content-Disposition HTTP header
   *
   * @param  string  $file     Filename
   * @param  string  $filemime MIME Type
   * @param  bool    $download Disposition type (true = download file, false = open file in browser)
   * @return string
   * @author Dmitry (dio) Levashov, Alexey Sukhotin
   * */
  public static function GetContentDisposition($file, $filemime, $download = FALSE) {

    $disp = '';
    $filename = $file;
    $ua = $_SERVER["HTTP_USER_AGENT"];
    $mime = $filemime;

    if ($download) {
      $disp = 'attachment';
      $mime = 'application/octet-stream';
    } else {
      $disp = preg_match('/^(image|text)/i', $mime)
              || $mime == 'application/x-shockwave-flash' ? 'inline' : 'attachment';
    }

    $disp .= '; ';

    if (preg_match("/MSIE ([0-9]{1,}[\.0-9]{0,})/", $ua)) {
      $filename = rawurlencode($filename);
      $filename = str_replace("+", "%20", $filename);
      //$filename = str_replace(" ", "%20", $filename);
      $disp .= "filename=" . $filename;
    } elseif (preg_match("/Firefox\/(\d+)/", $ua, $m)) {
      if ($m[1] >= 8) {
        $disp .= "filename*=?UTF-8''" . rawurlencode($filename);
      } else {
        $disp .= "filename*=\"?UTF-8''" . rawurlencode($filename) . "\";";
      }
    } else {
      $disp .= "filename=" . $filename;
    }

    return $disp;
  }

}