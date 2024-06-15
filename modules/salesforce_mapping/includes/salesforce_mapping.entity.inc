<?php

/**
 * @file
 * Contains SalesforceMapping.
 */

/**
 * Entity class for Salesforce Mappings
 */
class SalesforceMapping extends Entity {

  // Only one bundle type for now.
  public $type = 'salesforce_mapping';
  protected $entityType;
  protected $entityInfo;
  protected $idKey, $nameKey, $statusKey;
  protected $defaultLabel = FALSE;
  protected $wrapper;

  /**
   * Constructor for SalesforceMapping.
   *
   * @param array $values
   *   Associated array of values for the fields the entity should start with.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'salesforce_mapping');
    $this->entityType = $this->type;
    $this->setUp();
    // Set initial values.
    foreach ($values as $key => $value) {
      $this->$key = $value;
    }
  }

  /**
   * Save the entity.
   *
   * @return object
   *   The newly saved version of the entity.
   */
  public function save() {
    if (isset($this->is_new) && $this->is_new) {
      $this->created = REQUEST_TIME;
    }
    return parent::save();
  }

  protected function setUp() {
    $this->entityInfo = entity_get_info($this->entityType);
    $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : $this->idKey;
    $this->statusKey = empty($this->entityInfo['entity keys']['status']) ? 'status' : $this->entityInfo['entity keys']['status'];
  }
  /**
   * Return an array of Salesforce Field Names included in this mapping.
   *
   * @param array $directions
   *   If given, only include fields mapped in this or these directions.
   *   Possible values are:
   *     SALESFORCE_MAPPING_DIRECTION_SYNC
   *     SALESFORCE_MAPPING_DIRECTION_SF_BACKDROP
   *     SALESFORCE_MAPPING_DIRECTION_BACKDROP_SF
   *   @see salesforce_mapping.module
   * @return array $mapped_fields
   *   Indexes and keys are both Salesforce Field (machine) Name
   */
  public function getMappedFields(array $directions = NULL) {
    $mapped_fields = array();
    foreach ($this->field_mappings as $field_map) {
      if (empty($directions) || in_array($field_map['direction'], $directions)) {
        // Some field map types (Relation) store a collection of SF objects.
        if (is_array($field_map['salesforce_field']) && !isset($field_map['salesforce_field']['name'])) {
          foreach ($field_map['salesforce_field'] as $sf_field) {
            $mapped_fields[$sf_field['name']] = $sf_field['name'];
          }
        }
        // The rest of are just a name/value pair.
        else {
          $mapped_fields[$field_map['salesforce_field']['name']] = $field_map['salesforce_field']['name'];
        }
      }
    }

    if (!empty($this->getMappedRecordTypes())) {
      $mapped_fields['RecordTypeId'] = 'RecordTypeId';
    }

    return $mapped_fields;
  }

  public function entityInfo() {
    return salesforce_mapping_entity_info();
  }

  public function getMappedRecordTypes() {
    return $this->salesforce_record_type_default == SALESFORCE_MAPPING_DEFAULT_RECORD_TYPE
      ? array()
      : array_filter($this->salesforce_record_types_allowed);
  }

  /**
   * Retreive the default URI.
   *
   * @return array
   *   Associated array with the default URI on the 'path' key.
   */
  protected function defaultUri() {
    return array('path' => 'admin/structure/salesforce/mappings/manage/' . $this->identifier(),
                    'options'=>array(
                      'entity_type'=>$this->entityType,
                       'entity' => entity_load($this->entityType,$this->identifier()),
                    ));
  }

  public function id() {
    return $this->identifier();
  }

  public function entityType() {
    return $this->entityType;
  }

  public function label() {
    // If the default label flag is enabled, this is being invoked recursively.
    // In this case we need to use our default label callback directly. This may
    // happen if a module provides a label callback implementation different
    // from ours, but then invokes Entity::label() or entity_class_label() from
    // there.
    if ($this->defaultLabel || (isset($this->entityInfo['label callback']) && $this->entityInfo['label callback'] == 'entity_class_label')) {
      return $this->defaultLabel();
    }
    $this->defaultLabel = TRUE;
    $label = entity_label($this->entityType, $this);
    $this->defaultLabel = FALSE;
    return $label;
  }

  public function uri() {
    if (isset($this->entityInfo['uri callback']) && $this->entityInfo['uri callback'] == 'entity_class_uri') {
      return $this->defaultUri();
    }
    return entity_uri($this->entityType, $this);
  }

  function entity_uri($entity_type, $entity) {
    $info = entity_get_info($entity_type);
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

    // A bundle-specific callback takes precedence over the generic one for the
    // entity type.
    if (isset($info['bundles'][$bundle]['uri callback'])) {
      $uri_callback = $info['bundles'][$bundle]['uri callback'];
    }
    elseif (isset($info['uri callback'])) {
      $uri_callback = $info['uri callback'];
    }
    else {
      return NULL;
    }

    // Invoke the callback to get the URI. If there is no callback, return NULL.
    if (isset($uri_callback) && function_exists($uri_callback)) {
      $uri = $uri_callback($entity);
      // Pass the entity data to url() so that alter functions do not need to
      // lookup this entity again.
      $uri['options']['entity_type'] = $entity_type;
      $uri['options']['entity'] = $entity;
      return $uri;
    }
  }
  protected function defaultLabel() {
    // Add in the translated specified label property.
    return $this->getTranslation($this->entityInfo['entity keys']['label']);
  }

  public function getTranslation($property, $langcode = NULL) {
    $all_info = entity_plus_get_all_property_info($this->entityType);
    // Assign by reference to avoid triggering notices if metadata is missing.
    $property_info = &$all_info[$property];

    if (!empty($property_info['translatable'])) {
      if (!empty($property_info['field'])) {
        return field_get_items($this->entityType, $this, $property, $langcode);
      }
      elseif (!empty($property_info['i18n string'])) {
        $name = $this->entityInfo['module'] . ':' . $this->entityType . ':' . $this->identifier() . ':' . $property;
        return entity_plus_i18n_string($name, $this->$property, $langcode);
      }
    }
    return $this->$property;
  }
  public function identifier() {
    return isset($this->{$this->nameKey}) ? $this->{$this->nameKey} : NULL;
  }

}
