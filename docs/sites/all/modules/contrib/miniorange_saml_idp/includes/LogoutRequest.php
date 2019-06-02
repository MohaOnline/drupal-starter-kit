<?php
/**
 * @package    miniOrange
 * @subpackage Plugins
 * @license    GNU/GPLv3
 * @copyright  Copyright 2015 miniOrange. All Rights Reserved.
 *
 *
 * This file is part of miniOrange Joomla SAML IDP plugin.
 *
 * miniOrange Joomla SAML IDP plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * miniOrange Joomla IDP plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with miniOrange SAML plugin.  If not, see <http://www.gnu.org/licenses/>.
 */
include_once 'Utilities.php';
class SAML2_LogoutRequest
{
	private $tagName;
	private $id;
	private $issuer;
	private $destination;
	private $issueInstant;
	private $certificates;
	private $validators;
    private $notOnOrAfter;
    private $encryptedNameId;
    private $nameId;
    private $sessionIndexes;

    public function __construct(DOMElement $xml = NULL)
    {
        $this->tagName = 'LogoutRequest';

        $this->id = Utilities::generateID();
        $this->issueInstant = time();
        $this->certificates = array();
        $this->validators = array();

        if ($xml === NULL) {
            return;
        }

        if (!$xml->hasAttribute('ID')) {
            throw new Exception('Missing ID attribute on SAML message.');
        }
        $this->id = $xml->getAttribute('ID');

        if ($xml->getAttribute('Version') !== '2.0') {
            /* Currently a very strict check. */
            throw new Exception('Unsupported version: ' . $xml->getAttribute('Version'));
        }

        $this->issueInstant = Utilities::xsDateTimeToTimestamp($xml->getAttribute('IssueInstant'));

        if ($xml->hasAttribute('Destination')) {
            $this->destination = $xml->getAttribute('Destination');
        }

        $issuer = Utilities::xpQuery($xml, './saml_assertion:Issuer');
        if (!empty($issuer)) {
            $this->issuer = trim($issuer[0]->textContent);
        }

        /* Validate the signature element of the message. */
        try {
            $sig = Utilities::validateElement($xml);

            if ($sig !== FALSE) {
                $this->certificates = $sig['Certificates'];
                $this->validators[] = array(
                    'Function' => array('Utilities', 'validateSignature'),
                    'Data' => $sig,
                    );
            }

        } catch (Exception $e) {
            /* Ignore signature validation errors. */
        }

        $this->sessionIndexes = array();

        if ($xml->hasAttribute('NotOnOrAfter')) {
            $this->notOnOrAfter = Utilities::xsDateTimeToTimestamp($xml->getAttribute('NotOnOrAfter'));
        }

        $nameId = Utilities::xpQuery($xml, './saml_assertion:NameID | ./saml_assertion:EncryptedID/xenc:EncryptedData');
        if (empty($nameId)) {
            throw new Exception('Missing <saml:NameID> or <saml:EncryptedID> in <samlp:LogoutRequest>.');
        } elseif (count($nameId) > 1) {
            throw new Exception('More than one <saml:NameID> or <saml:EncryptedD> in <samlp:LogoutRequest>.');
        }
        $nameId = $nameId[0];
        if ($nameId->localName === 'EncryptedData') {
            /* The NameID element is encrypted. */
            $this->encryptedNameId = $nameId;
        } else {
            $this->nameId = Utilities::parseNameId($nameId);
        }

        $sessionIndexes = Utilities::xpQuery($xml, './saml_protocol:SessionIndex');
        foreach ($sessionIndexes as $sessionIndex) {
            $this->sessionIndexes[] = trim($sessionIndex->textContent);
        }
    }

    /**
     * Retrieve the expiration time of this request.
     *
     * @return int|NULL The expiration time of this request.
     */
    public function getNotOnOrAfter()
    {
        return $this->notOnOrAfter;
    }

    /**
     * Set the expiration time of this request.
     *
     * @param int|NULL $notOnOrAfter The expiration time of this request.
     */
    public function setNotOnOrAfter($notOnOrAfter)
    {
        $this->notOnOrAfter = $notOnOrAfter;
    }

    /**
     * Check whether the NameId is encrypted.
     *
     * @return TRUE if the NameId is encrypted, FALSE if not.
     */
    public function isNameIdEncrypted()
    {
        if ($this->encryptedNameId !== NULL) {
            return TRUE;
        }

        return FALSE;
    }

    /**
     * Encrypt the NameID in the LogoutRequest.
     *
     * @param XMLSecurityKey $key The encryption key.
     */
    public function encryptNameId(XMLSecurityKey $key)
    {
        /* First create a XML representation of the NameID. */
        $doc = new DOMDocument();
        $root = $doc->createElement('root');
        $doc->appendChild($root);
        SAML2_Utils::addNameId($root, $this->nameId);
        $nameId = $root->firstChild;

        SAML2_Utils::getContainer()->debugMessage($nameId, 'encrypt');

        /* Encrypt the NameID. */
        $enc = new XMLSecEnc();
        $enc->setNode($nameId);
        $enc->type = XMLSecEnc::Element;

        $symmetricKey = new XMLSecurityKey(XMLSecurityKey::AES128_CBC);
        $symmetricKey->generateSessionKey();
        $enc->encryptKey($key, $symmetricKey);

        $this->encryptedNameId = $enc->encryptNode($symmetricKey);
        $this->nameId = NULL;
    }

    /**
     * Decrypt the NameID in the LogoutRequest.
     *
     * @param XMLSecurityKey $key       The decryption key.
     * @param array          $blacklist Blacklisted decryption algorithms.
     */
    public function decryptNameId(XMLSecurityKey $key, array $blacklist = array())
    {
        if ($this->encryptedNameId === NULL) {
            /* No NameID to decrypt. */

            return;
        }

        $nameId = SAML2_Utils::decryptElement($this->encryptedNameId, $key, $blacklist);
        SAML2_Utils::getContainer()->debugMessage($nameId, 'decrypt');
        $this->nameId = SAML2_Utils::parseNameId($nameId);

        $this->encryptedNameId = NULL;
    }

    /**
     * Retrieve the name identifier of the session that should be terminated.
     *
     * @return array The name identifier of the session that should be terminated.
     * @throws Exception
     */
    public function getNameId()
    {
        if ($this->encryptedNameId !== NULL) {
            throw new Exception('Attempted to retrieve encrypted NameID without decrypting it first.');
        }

        return $this->nameId;
    }

    /**
     * Set the name identifier of the session that should be terminated.
     *
     * The name identifier must be in the format accepted by SAML2_message::buildNameId().
     *
     * @see SAML2_message::buildNameId()
     * @param array $nameId The name identifier of the session that should be terminated.
     */
    public function setNameId($nameId)
    {
        $this->nameId = $nameId;
    }

    /**
     * Retrieve the SessionIndexes of the sessions that should be terminated.
     *
     * @return array The SessionIndexes, or an empty array if all sessions should be terminated.
     */
    public function getSessionIndexes()
    {
        return $this->sessionIndexes;
    }

    /**
     * Set the SessionIndexes of the sessions that should be terminated.
     *
     * @param array $sessionIndexes The SessionIndexes, or an empty array if all sessions should be terminated.
     */
    public function setSessionIndexes(array $sessionIndexes)
    {
        $this->sessionIndexes = $sessionIndexes;
    }

    /**
     * Retrieve the sesion index of the session that should be terminated.
     *
     * @return string|NULL The sesion index of the session that should be terminated.
     */
    public function getSessionIndex()
    {
        if (empty($this->sessionIndexes)) {
            return NULL;
        }

        return $this->sessionIndexes[0];
    }

    /**
     * Set the sesion index of the session that should be terminated.
     *
     * @param string|NULL $sessionIndex The sesion index of the session that should be terminated.
     */
    public function setSessionIndex($sessionIndex)
    {
        if (is_null($sessionIndex)) {
            $this->sessionIndexes = array();
        } else {
            $this->sessionIndexes = array($sessionIndex);
        }
    }

    /**
     * Convert this logout request message to an XML element.
     *
     * @return DOMElement This logout request.
     */
    public function toUnsignedXML()
    {
        $root = parent::toUnsignedXML();

        if ($this->notOnOrAfter !== NULL) {
            $root->setAttribute('NotOnOrAfter', gmdate('Y-m-d\TH:i:s\Z', $this->notOnOrAfter));
        }

        if ($this->encryptedNameId === NULL) {
            SAML2_Utils::addNameId($root, $this->nameId);
        } else {
            $eid = $root->ownerDocument->createElementNS(SAML2_Const::NS_SAML, 'saml:' . 'EncryptedID');
            $root->appendChild($eid);
            $eid->appendChild($root->ownerDocument->importNode($this->encryptedNameId, TRUE));
        }

        foreach ($this->sessionIndexes as $sessionIndex) {
            SAML2_Utils::addString($root, SAML2_Const::NS_SAMLP, 'SessionIndex', $sessionIndex);
        }

        return $root;
    }

    public function getId()
    {
        return $this->id;
    }
    
    public function setId($id)
    {
        $this->id = $id;
    }
    
    public function getIssueInstant()
    {
        return $this->issueInstant;
    }

    public function setIssueInstant($issueInstant)
    {
        $this->issueInstant = $issueInstant;
    }

    public function getDestination()
    {
        return $this->destination;
    }

    public function setDestination($destination)
    {
		$this->destination = $destination;
    }

    public function getIssuer()
    {
        return $this->issuer;
    }

    public function setIssuer($issuer)
    {
       $this->issuer = $issuer;
    }
}
