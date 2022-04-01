/**
 * mo-tracking - A tracking module.
 * @version v0.1.0
 * @link https://github.com/moreonion/tracking
 * @license AGPL-3.0
 */
(function (global, factory) {
    if (typeof define === "function" && define.amd) {
        define(['exports'], factory);
    } else if (typeof exports !== "undefined") {
        factory(exports);
    } else {
        var mod = {
            exports: {}
        };
        factory(mod.exports);
        global.tracking = mod.exports;
    }
})(this, function (exports) {
    'use strict';

    Object.defineProperty(exports, "__esModule", {
        value: true
    });
    exports.Tracker = Tracker;

    var _extends = Object.assign || function (target) {
        for (var i = 1; i < arguments.length; i++) {
            var source = arguments[i];

            for (var key in source) {
                if (Object.prototype.hasOwnProperty.call(source, key)) {
                    target[key] = source[key];
                }
            }
        }

        return target;
    };

    /* global window global */
    /**
     * Tracking module.
     *
     * @module tracking
     */

    var root;
    if (typeof window !== 'undefined') {
        root = window;
    } else if (typeof global !== 'undefined') {
        root = global;
    } else {
        root = {};
    }

    var MARKED_VALUE = '1';
    var UNMARKED_VALUE = '0';

    /**
     * Creates a Tracker instance.
     *
     * @constructor
     * @this {Tracker}
     * @param {object} [options] - the options
     * @param {string} [options.root=window|global|{}] - the root object<br>
     *     Per default tries to set this to <code>window</code> (Browsers),
     *     <code>global</code> (nodejs), or an empty object <code>{}</code>.
     * @param {string} [options.gaFnName='ga'] - Google Analytics command queue
     *     function name
     * @param {string} [options.uniqueEvents=true] - send events only once
     * @param {function}
     *     [options.storageIdentifierFn=this.defaultStorageIdentifierFn] - function to
     *     determine the storage identifier of an action<br>
     *     <br>
     *     The <code>storageIdentifierFn</code> needs at least an
     *     <code>actionName</code> (string) as first parameter. Optional parameter
     *     object which can be used in determining the storage key are:
     *     <code>object</code>, <code>event</code>, and <code>action</code>.
     * @param {function} [options.uniquifyFn=this.defaultUniquifyingFn] - function to
     *     determine if something should count as unique compared to the.<br>
     *     Default is to use <code>window.location.pathname</code>
     * @param {string} [options.storage='local'] - the storage to use, for now<br>
     *     This could be <code>local</code> for <code>root.localStorage</code> or
     *     <code>session</code> for <code>root.sessionStorage</code>
     * @param {object} [options.eventDefaults={}] - the default event
     * @param {string} [options.eventDefaults.hitType='event'] - the default hit type
     * @param {string} [options.eventDefaults.eventCategory] - the default event category
     * @param {string} [options.eventDefaults.eventAction] - the default event action
     * @param {string} [options.eventDefaults.eventLabel] - the default event label
     * @param {number} [options.eventDefaults.eventValue] - the default event value
     * @param {string} [options.defaultCurrency='EUR'] - the default currency for enhanced ecommerce
     * @public
     * @todo make storage customizable with own implementation
     * @todo factor out storage into own module
     * @todo check for window.GoogleAnalyticsObject to set gaFnName
     * @todo load from storage on a possible second page/step?
     */
    function Tracker(options) {
        /**
         * The default settings.
         * @var {object}
         * @inner
         */
        var defaults = {
            root: root,
            gaFnName: 'ga',
            storage: 'local',
            defaultCurrency: 'EUR',
            storageIdentifierFn: this.defaultStorageIdentifierFn,
            uniquifyFn: this.defaultUniquifyingFn,
            uniqueEvents: true,
            eventDefaults: {
                hitType: 'event'
            }
        };

        /**
         * The active settings.
         * @member {object} settings
         * @instance
         * @memberof module:tracking~Tracker
         */
        this.settings = _extends(defaults, options);

        /**
         * The state storage to use.
         *
         * Can be `localStorage` or `sessionStorage`
         *
         * @member {Storage} storage to use
         * @instance
         * @memberof module:tracking~Tracker
         * @todo add storage `cookie`
         */
        this.storage = null;

        if (this.settings.storage === 'local') {
            this.storage = this.settings.root['localStorage'];
        }
        if (this.settings.storage === 'session') {
            this.storage = this.settings.root['sessionStorage'];
        }

        /**
         * The Google Analytics command queue to use
         *
         * @member {function} ga() to use
         * @instance
         * @memberof module:tracking~Tracker
         */
        this.ga = this._ensureGA();
    }

    /**
     * Ensure that the Google Analytics command queue (or a dummy) exists.
     *
     * The dummy is the same as is used in GA snippets until the actual script
     * is loaded.
     *
     * @private
     * @returns {boolean}
     */
    Tracker.prototype._ensureGA = function () {
        (function(i,r){i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)}})(this.settings.root, this.settings.gaFnName);
        return this.settings.root[this.settings.gaFnName];
    };

    /**
     * Ensure the plugin is loaded.
     *
     * @private
     */
    Tracker.prototype._ensurePlugin = function () {
        var name = arguments.length <= 0 || arguments[0] === undefined ? 'ec' : arguments[0];

        this.ga('require', name);
    };

    /**
     * Set basic properties for donation.
     *
     * @param {object} [options] - the options
     * @param {string} [options.defaultCurrency='EUR'] - the default currency for enhanced ecommerce
     */
    Tracker.prototype.initializeDonation = function (options) {
        var defaults = {
            currency: this.settings.defaultCurrency
        };
        var settings = _extends(defaults, options);
        this._ensurePlugin('ec');
        this.ga('set', '&cu', settings.currency);
    };

    /**
     * Validate an object.
     *
     * @param {string} [type='event'] - the type to validate against<br>
     *     Can be on of: <code>event</code>, <code>impression</code>,
     *     <code>product</code>, <code>action</action>, or <code>purchase</code>.
     * @param {object} [object={}] - the object to validate
     * @private
     * @see https://developers.google.com/analytics/devguides/collection/analyticsjs/field-reference
     */
    Tracker.prototype._validateObject = function () {
        var type = arguments.length <= 0 || arguments[0] === undefined ? 'event' : arguments[0];
        var object = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

        // check if we really get an object to validate against
        if (typeof object !== 'object' || object === null) {
            return false;
        }

        // initialize
        var success = true;
        var oneOfAttributesRequired = [];
        var allAttributesRequired = [];

        // setup requirements
        if (type === 'event') {
            // see https://developers.google.com/analytics/devguides/collection/analyticsjs/events#event_fields
            allAttributesRequired = ['eventCategory', 'eventAction'];
        } else if (type === 'impression' || type === 'product') {
            // see https://developers.google.com/analytics/devguides/collection/analyticsjs/enhanced-ecommerce#impression-data
            // https://developers.google.com/analytics/devguides/collection/analyticsjs/enhanced-ecommerce#product-data
            oneOfAttributesRequired = ['id', 'name'];
        } else if (type === 'action') {
            success = true;
        } else if (type === 'purchase') {
            // see https://developers.google.com/analytics/devguides/collection/analyticsjs/enhanced-ecommerce#action-data
            allAttributesRequired = ['id'];
        }

        if (oneOfAttributesRequired.length > 0) {
            // one-of needs reverted start state (false)
            success = false;
            oneOfAttributesRequired.forEach(function (el) {
                if (el in object) {
                    success = true;
                }
            });
        }
        allAttributesRequired.forEach(function (el) {
            success = success && el in object;
        });

        return success;
    };

    /**
     * Send an impression.
     *
     * @param {impressionFieldObject} [object] - the
     *     <code>impressionFieldObject</code> to send
     * @param {event} [object] - the event to send
     */
    Tracker.prototype.sendImpression = function (impression) {
        var event = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

        var ev = _extends(this.settings.eventDefaults, {
            eventCategory: 'donation',
            eventAction: 'impression'
        });
        var donationEvent = _extends(ev, event);
        if (this._validateObject('impression', impression) && this._validateObject('event', donationEvent)) {
            if (this.settings.uniqueEvents && !this.isSent('impression', impression, donationEvent)) {
                this.ga('ec:addImpression', impression);
                this.ga('send', donationEvent);
                this.markSent('impression', impression, donationEvent);
            }
        } else {
            console.error('Tracker: not a valid impressionFieldObject and/or event.'); // eslint-disable-line no-console
        }
    };

    /**
     * Send a detail view.
     *
     * @param {productFieldObject} [object] - the
     *     <code>productFieldObject</code> to send
     * @param {event} [object] - the event to send
     */
    Tracker.prototype.sendView = function (product) {
        var event = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

        var ev = _extends(this.settings.eventDefaults, {
            eventCategory: 'donation',
            eventAction: 'view'
        });
        var donationEvent = _extends(ev, event);
        if (this._validateObject('product', product) && this._validateObject('event', donationEvent)) {
            if (this.settings.uniqueEvents && !this.isSent('view', product, donationEvent)) {
                this.ga('ec:addProduct', product);
                this.ga('ec:setAction', 'detail');
                this.ga('send', donationEvent);
                this.markSent('view', product, donationEvent);
            }
        } else {
            console.error('Tracker: not a valid productFieldObject and/or event.'); // eslint-disable-line no-console
        }
    };

    /**
     * Send a product add.
     *
     * @param {productFieldObject} [object] - the
     *     <code>productFieldObject</code> to send
     * @param {event} [object] - the event to send
     */
    Tracker.prototype.sendAdd = function (product) {
        var event = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

        var ev = _extends(this.settings.eventDefaults, {
            eventCategory: 'donation',
            eventAction: 'add to cart'
        });
        var donationEvent = _extends(ev, event);
        if (this._validateObject('product', product) && this._validateObject('event', donationEvent)) {
            if (this.settings.uniqueEvents && !this.isSent('add', product, donationEvent)) {
                this.ga('ec:addProduct', product);
                this.ga('ec:setAction', 'add');
                this.ga('send', donationEvent);
                this.markSent('add', product, donationEvent);
            }
        } else {
            console.error('Tracker: not a valid productFieldObject and/or event.'); // eslint-disable-line no-console
        }
    };

    /**
     * Send a checkout begin.
     *
     * @param {productFieldObject} [object] - the
     *     <code>productFieldObject</code> to send
     * @param {event} [object] - the event to send
     * @param {action} [object] - the action parameters
     */
    Tracker.prototype.sendBeginCheckout = function (product) {
        var event = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var action = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];

        var ev = _extends(this.settings.eventDefaults, {
            eventCategory: 'donation',
            eventAction: 'checkout'
        });
        var donationEvent = _extends(ev, event);
        var actionDefaults = {
            step: 1
        };
        var checkoutAction = _extends(actionDefaults, action);
        if (this._validateObject('product', product) && this._validateObject('event', donationEvent) && this._validateObject('action', checkoutAction)) {
            if (this.settings.uniqueEvents && !this.isSent('checkoutBegin', product, donationEvent, checkoutAction)) {
                this.ga('ec:addProduct', product);
                this.ga('ec:setAction', 'checkout', checkoutAction);
                this.ga('send', donationEvent);
                this.markSent('checkoutBegin', product, donationEvent, checkoutAction);
            }
        } else {
            console.error('Tracker: not a valid productFieldObject and/or event and/or checkout action.'); // eslint-disable-line no-console
        }
    };

    /**
     * Send a checkout end.
     *
     * @param {productFieldObject} [object] - the
     *     <code>productFieldObject</code> to send
     * @param {event} [object] - the event to send
     * @param {action} [object] - the action parameters
     */
    Tracker.prototype.sendEndCheckout = function (product) {
        var event = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var action = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];

        var ev = _extends(this.settings.eventDefaults, {
            eventCategory: 'donation',
            eventAction: 'checkout'
        });
        var donationEvent = _extends(ev, event);
        var actionDefaults = {
            step: 2
        };
        var checkoutAction = _extends(actionDefaults, action);
        if (this._validateObject('product', product) && this._validateObject('event', donationEvent) && this._validateObject('action', checkoutAction)) {
            if (this.settings.uniqueEvents && !this.isSent('checkoutEnd', product, donationEvent, checkoutAction)) {
                this.ga('ec:addProduct', product);
                this.ga('ec:setAction', 'checkout', checkoutAction);
                this.ga('send', donationEvent);
                this.markSent('checkoutEnd', product, donationEvent, checkoutAction);
            }
        } else {
            console.error('Tracker: not a valid productFieldObject and/or event and/or checkout action.'); // eslint-disable-line no-console
        }
    };

    /**
     * Send a purchase.
     *
     * @param {productFieldObject} [object] - the
     *     <code>productFieldObject</code> to send
     * @param {event} [object] - the event to send
     * @param {action} [object] - the action parameters
     */
    Tracker.prototype.sendPurchase = function (product) {
        var event = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var action = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];

        var ev = _extends(this.settings.eventDefaults, {
            eventCategory: 'donation',
            eventAction: 'purchase'
        });
        var donationEvent = _extends(ev, event);
        var actionDefaults = {
            currency: this.settings.defaultCurrency
        };
        var purchaseAction = _extends(actionDefaults, action);
        if (this._validateObject('product', product) && this._validateObject('event', donationEvent) && this._validateObject('purchase', purchaseAction)) {
            if (this.settings.uniqueEvents && !this.isSent('purchase', product, donationEvent, purchaseAction)) {
                this.ga('ec:addProduct', product);
                this.ga('ec:setAction', 'purchase', purchaseAction);
                this.ga('send', donationEvent);
                this.markSent('purchase', product, donationEvent, purchaseAction);
            }
        } else {
            console.error('Tracker: not a valid productFieldObject and/or event and/or purchase action.'); // eslint-disable-line no-console
        }
    };

    /**
     * Default uniquifiying function.
     *
     * @returns {string}
     */
    Tracker.prototype.defaultUniquifyingFn = function () {
        if ('location' in root) {
            return root.location.pathname;
        }
    };

    /**
     * Default storage identifier function.
     *
     * @param {string} actionName - the action name
     * @param {object} [object={}] - the object, i.e. product, impression
     * @returns {string}
     */
    Tracker.prototype.defaultStorageIdentifierFn = function (actionName) {
        var object = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

        var prefix = this.settings.uniquifyFn.call(this);
        var objectId = typeof object.id === 'undefined' ? '' : object.id;
        return prefix + '::' + actionName + '::' + objectId;
    };

    /**
     * Check if an action was sent.
     *
     * @param {string} actionName - the action to check
     * @param {object} [object={}] - the object, i.e. product, impression
     * @param {event} [event={}] - the event parameters
     * @param {action} [action={}] - the action parameters
     * @returns {boolean}
     */
    Tracker.prototype.isSent = function (actionName) {
        var object = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var event = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];
        var action = arguments.length <= 3 || arguments[3] === undefined ? {} : arguments[3];

        var identifier = this.settings.storageIdentifierFn.call(this, actionName, object, event, action);
        var val = this.storage.getItem(identifier);
        if (val === MARKED_VALUE) {
            return true;
        } else {
            return false;
        }
    };

    /**
     * Mark an action as sent.
     *
     * @param {string} actionName - the action to mark sent
     * @param {object} [object={}] - the object, i.e. product, impression
     * @param {event} [event={}] - the event parameters
     * @param {action} [action={}] - the action parameters
     */
    Tracker.prototype.markSent = function (actionName) {
        var object = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var event = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];
        var action = arguments.length <= 3 || arguments[3] === undefined ? {} : arguments[3];

        var identifier = this.settings.storageIdentifierFn.call(this, actionName, object, event, action);
        this.storage.setItem(identifier, MARKED_VALUE);
    };

    /**
     * Unmark an action as sent.
     *
     * @param {string} actionName - the action to unmark sent
     * @param {object} [object={}] - the object, i.e. product, impression
     * @param {event} [event={}] - the event parameters
     * @param {action} [action={}] - the action parameters
     */
    Tracker.prototype.unmarkSent = function (actionName) {
        var object = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var event = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];
        var action = arguments.length <= 3 || arguments[3] === undefined ? {} : arguments[3];

        var identifier = this.settings.storageIdentifierFn.call(this, actionName, object, event, action);
        this.storage.setItem(identifier, UNMARKED_VALUE);
    };

    /**
     * Remove an action state from storage.
     *
     * @param {string} actionName - the action state to remove
     * @param {object} [object={}] - the object, i.e. product, impression
     * @param {event} [event={}] - the event parameters
     * @param {action} [action={}] - the action parameters
     */
    Tracker.prototype.removeSent = function (actionName) {
        var object = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
        var event = arguments.length <= 2 || arguments[2] === undefined ? {} : arguments[2];
        var action = arguments.length <= 3 || arguments[3] === undefined ? {} : arguments[3];

        var identifier = this.settings.storageIdentifierFn.call(this, actionName, object, event, action);
        this.storage.removeItem(identifier);
    };

    /**
     * Reset the storage.
     */
    Tracker.prototype.resetStorage = function () {
        this.storage.clear();
    };

    // vim: set et ts=4 sw=4 :
});
