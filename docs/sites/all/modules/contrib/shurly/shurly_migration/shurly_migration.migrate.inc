<?php

/**
 * Abstract intermediate class holding common settings.
 */
abstract class CommonMigration extends Migration {

    public function __construct($arguments) {
        parent::__construct($arguments);

        $this->team = array(
            new MigrateTeamMember('Alexander Wanke', 'alexander.wanke@a8w4.de', t('a8w4.de')),
        );
        $this->issuePattern = 'http://drupal.org/node/:id:';
    }
}

/**
 * Class ShortURLMigration
 * migrates from Shurly Drupal 6 module
 */

class ShortURLMigration extends CommonMigration {

    public function __construct($arguments) {
        parent::__construct($arguments);
        $this->description = 'Short URLs to Shurly Migration';

        $table_name = 'shurly';

        $query = db_select('shorturl_link', 't')
            ->fields('t', array('lid', 'orig_url', 'created'));
        $this->source = new MigrateSourceSQL($query);
        $this->destination = new MigrateDestinationTable($table_name);
        $this->map = new MigrateSQLMap($this->machineName,
                array('lid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
            )
            ),
            MigrateDestinationTable::getKeySchema($table_name)
        );

        // Mapped fields
        $this->addFieldMapping('source', 'lid')->callbacks(array($this, 'shortUrlDecode'))->defaultValue('unprocessed');
        $this->addFieldMapping('destination', 'orig_url');
        $this->addFieldMapping('created', 'created');
        $this->addFieldMapping('last_used')->defaultValue(0);
        $this->addFieldMapping('count')->defaultValue(0);
        $this->addFieldMapping('custom')->defaultValue(0);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('active')->defaultValue(1);

        $this->addUnmigratedDestinations(array('rid'));
    }

    /**
     * Encode number (index of the URL) into url using alpha-numeric mapping
     * (basically copied from shorturl.module)
     */
    protected function shortUrlDecode ($lid) {

        $mapping = $this->shorturl_base_encode_mapping();
        $lid = $this->shorturl_base_encode($lid, sizeof($mapping));
        $arr = preg_split('/-/', $lid);

        if (empty($arr)) { return $mapping[0]; }
        if (!is_array($arr)) { $arr = array($arr); }

        $new_num = "";
        foreach ($arr as $pos)  {
            $new_num .= $mapping[(int)trim($pos)];
        }

        return $new_num;

    }

    /**
     * Convert $var from (10) base notation to $base base notation.
     */
    private function shorturl_base_encode($var, $base)	{	// return string, accept two integers
        if ($var < $base)	{ // see if it's time to return
            return $var;
        }
        else	{
            return $this->shorturl_base_encode(floor($var/$base), $base) .  "-" . ($var % $base);	// continue
        }
    }
    /**
     * The very first 4-char number is 238328
     * The very first 3-char number is 3844
     */
    private function shorturl_base_encode_mapping() {
        return array(
            'q','l','i','3','O','c','x','a','C','1','r','Y',
            'g','4','2','T','b','j','D','W','Z','B','K','k',
            '0','8','9','X','I','5','N','R','n','Q','U','P',
            'E','m','y','V','p','d','v','w','f','G','7','A',
            'o','s','H','6','F','L','M','e','t','z','u','J',
            'S','h'
        );
    }

}
