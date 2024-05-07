<?php

/**
 * @file
 * Contains SalesforceMappingObject.
 */

/**
 * Entity class for Salesforce Mapping Objects
 */
class SalesforceMappingObject extends Entity {

  // @codingStandardsIgnoreStart
  public
    $salesforce_mapping_object_id,
    $revision_id,
    $salesforce_id,
    $entity_id,
    $entity_type,
    $created,
    $entity_updated,
    $last_sync,
    $last_sync_action,
    $last_sync_status,
    $last_sync_message;
  // @codingStandardsIgnoreEnd

  protected $entityType;
  protected $entityInfo;
  protected $idKey, $nameKey, $statusKey;
  protected $defaultLabel = FALSE;
  protected $wrapper;

  /**
   * Constructor for SalesforceMappingObject.
   *
   * @param array $values
   *   Associated array of values this entity should start with.
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'salesforce_mapping_object');
    $this->entityType = $entityType;
    $this->setUp();
    // Set initial values.
    foreach ($values as $key => $value) {
      $this->$key = $value;
    }
  }

  /**
   * Retrieve the default URI.
   *
   * @return array
   *   Associated array with the default URI on the 'path' key.
   */
  protected function defaultUri() {
    $uri = NULL;

    $wrapper = entity_metadata_wrapper($this->entityType(), $this);
    $entity = $wrapper->entity->value();
    if ($entity) {
      $uri = method_exists($entity, 'uri') ? $entity->uri() : entity_uri($this->entity_type, $entity);
    }
    if (is_null($uri)) {
      $path = 'admin/content/salesforce/' . $this->entity_type . '/' . $this->entity_id;
    }
    else {
      $path = $uri['path'];
    }
    return array('path' => $path . '/salesforce_activity');
  }

  /**
   * Retrieve the default label.
   */
  protected function defaultLabel() {
    if (isset($this->is_new) && $this->is_new === TRUE) {
      return '';
    }
    $label = NULL;
    $wrapper = entity_metadata_wrapper('salesforce_mapping_object', $this);
    $label = $wrapper->entity->label();
    $label = ($label) ? $label : $this->entity_type . ':' . $this->entity_id;
    return t('"@label" to "@sf_id"', array('@label' => $label, '@sf_id' => $this->salesforce_id));
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

    if (!isset($this->is_new_revision)) {
      $this->is_new_revision = TRUE;
    }

    if (!isset($this->default_revision)) {
      $this->default_revision = TRUE;
    }

    return parent::save();
  }

  protected function setUp() {
    $this->entityInfo = entity_get_info($this->entityType);
    $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : $this->idKey;
    $this->statusKey = empty($this->entityInfo['entity keys']['status']) ? 'status' : $this->entityInfo['entity keys']['status'];
  }

  public function entityInfo() {
    return salesforce_mapping_entity_info();
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
