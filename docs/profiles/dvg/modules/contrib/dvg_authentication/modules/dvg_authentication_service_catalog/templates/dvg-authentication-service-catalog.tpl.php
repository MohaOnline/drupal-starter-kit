<?php

/**
 * @file
 * Theme implementation to generate the catalog XML for the Service Catalog.
 *
 * Available variables:
 * - $service_catalog_uuid
 *    UUID for the service catalog.
 * - $issue_instant
 *    Date of creation of this service catalog.
 * - $oin:
 *    The OverheidsIdentificatieNummer, identifier of the municipality.
 * - $organisation_name
 *    Name of the organisation.
 * - $organisation_url
 *    Url of the website.
 * - $idp
 *    Identifier of the IdentityProvider, can be found in the broker's metadata.
 * - $certificate_name
 *   Name of the certificate used for saml, required for eIDAS.
 * - $certificate_content
 *   The certificate used for saml, required for eIDAS.
 * - $services: Array with all services to render, containing
 *   - id: machine name used to identify the level, don't use spaces.
 *   - serviceUUID: Unique Universal identifier for this service.
 *   - instanceUUID: Unique Universal identifier for this service.
 *   - level: identifier of the eHerkenning level, see:
 *       https://afsprakenstelsel.etoegang.nl/display/as/Level+of+assurance
 *   - name: Array with human readable names of the service, keyed by
 *       language code, identifying the type, level and environment of
 *       the service.
 *   - description: Array with human readable descriptions, keyed by
 *       language code.
 *   - type: The type of authentication: eidas or eherkenning.
 *
 * @see template_preprocess_dvg_authentication_login_button()
 */
?>
<?php print '<?xml version="1.0" encoding="UTF-8"?>'; ?>

<esc:ServiceCatalogue ID="<?php print $service_catalog_uuid; ?>" esc:IssueInstant="<?php print $issue_instant; ?>" esc:Version="urn:etoegang:1.10:53" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" xmlns:esc="urn:etoegang:1.11:service-catalog" xmlns:md="urn:oasis:names:tc:SAML:2.0:metadata" xmlns:saml2="urn:oasis:names:tc:SAML:2.0:assertion">
  <esc:ServiceProvider esc:IsPublic="true">
    <esc:ServiceProviderID><?php print $oin ?></esc:ServiceProviderID>
    <esc:OrganizationDisplayName xml:lang="nl"><?php print $organisation_name ?></esc:OrganizationDisplayName>
<?php foreach ($services as $service): ?>
    <esc:ServiceDefinition esc:IsPublic="true">
      <esc:ServiceUUID><?php print $service['serviceUUID']; ?></esc:ServiceUUID>
      <esc:ServiceName xml:lang="nl"><?php print $service['name']['nl']; ?></esc:ServiceName>
      <esc:ServiceName xml:lang="en"><?php print $service['name']['en']; ?></esc:ServiceName>
      <esc:ServiceDescription xml:lang="nl"><?php print $service['description']['nl']; ?></esc:ServiceDescription>
      <esc:ServiceDescription xml:lang="en"><?php print $service['description']['en']; ?></esc:ServiceDescription>
      <esc:ServiceDescriptionURL xml:lang="nl"><?php print $organisation_url; ?></esc:ServiceDescriptionURL>
      <saml2:AuthnContextClassRef><?php print $service['level']; ?></saml2:AuthnContextClassRef>
      <esc:HerkenningsmakelaarId><?php print $idp; ?></esc:HerkenningsmakelaarId>
<?php   if ($service['type'] === 'eherkenning'): ?>
      <esc:EntityConcernedTypesAllowed>urn:etoegang:1.9:EntityConcernedID:KvKnr</esc:EntityConcernedTypesAllowed>
      <esc:ServiceRestrictionsAllowed>urn:etoegang:1.9:ServiceRestriction:Vestigingsnr</esc:ServiceRestrictionsAllowed>
<?php   endif; ?>
<?php   if ($service['type'] === 'eidas'): ?>
      <esc:EntityConcernedTypesAllowed>urn:etoegang:1.9:EntityConcernedID:Pseudo</esc:EntityConcernedTypesAllowed>
      <esc:RequestedAttribute Name="urn:etoegang:1.9:attribute:Initials" isRequired="false">
        <esc:PurposeStatement xml:lang="nl">Nodig voor identificatie</esc:PurposeStatement>
        <esc:PurposeStatement xml:lang="en">Needed for identification</esc:PurposeStatement>
      </esc:RequestedAttribute>
      <esc:RequestedAttribute Name="urn:etoegang:1.9:attribute:FirstName" isRequired="true">
        <esc:PurposeStatement xml:lang="nl">Nodig voor identificatie</esc:PurposeStatement>
        <esc:PurposeStatement xml:lang="en">Needed for identification</esc:PurposeStatement>
      </esc:RequestedAttribute>
      <esc:RequestedAttribute Name="urn:etoegang:1.9:attribute:FamilyNameInfix" isRequired="true">
        <esc:PurposeStatement xml:lang="nl">Nodig voor identificatie</esc:PurposeStatement>
        <esc:PurposeStatement xml:lang="en">Needed for identification</esc:PurposeStatement>
      </esc:RequestedAttribute>
      <esc:RequestedAttribute Name="urn:etoegang:1.9:attribute:FamilyName" isRequired="true">
        <esc:PurposeStatement xml:lang="nl">Nodig voor identificatie</esc:PurposeStatement>
        <esc:PurposeStatement xml:lang="en">Needed for identification</esc:PurposeStatement>
      </esc:RequestedAttribute>
      <esc:RequestedAttribute Name="urn:etoegang:1.9:attribute:DateOfBirth" isRequired="true">
        <esc:PurposeStatement xml:lang="nl">Nodig voor identificatie</esc:PurposeStatement>
        <esc:PurposeStatement xml:lang="en">Needed for identification</esc:PurposeStatement>
      </esc:RequestedAttribute>
<?php   endif; ?>
    </esc:ServiceDefinition>
<?php endforeach; ?>
<?php foreach ($services as $service): ?>
    <esc:ServiceInstance esc:IsPublic="true">
      <esc:ServiceID><?php print $service['serviceID']; ?></esc:ServiceID>
      <esc:ServiceUUID><?php print $service['instanceUUID']; ?></esc:ServiceUUID>
      <esc:InstanceOfService><?php print $service['serviceUUID']; ?></esc:InstanceOfService>
<?php   if ($service['type'] === 'eidas'): ?>
      <esc:PrivacyPolicyURL xml:lang="nl"><?php print $privacy_policy_url; ?></esc:PrivacyPolicyURL>
<?php   endif; ?>
      <esc:HerkenningsmakelaarId><?php print $idp; ?></esc:HerkenningsmakelaarId>
      <esc:SSOSupport>false</esc:SSOSupport>
<?php   if ($service['type'] === 'eidas'): ?>
      <esc:ServiceCertificate>
        <md:KeyDescriptor use="encryption">
          <ds:KeyInfo>
            <ds:KeyName><?php print $certificate_name; ?></ds:KeyName>
            <ds:X509Data>
              <ds:X509Certificate><?php print $certificate_content; ?></ds:X509Certificate>
            </ds:X509Data>
          </ds:KeyInfo>
        </md:KeyDescriptor>
      </esc:ServiceCertificate>
      <esc:Classifiers>
          <esc:Classifier>eIDAS-inbound</esc:Classifier>
      </esc:Classifiers>
<?php   endif; ?>
    </esc:ServiceInstance>
<?php endforeach; ?>
  </esc:ServiceProvider>
</esc:ServiceCatalogue>
