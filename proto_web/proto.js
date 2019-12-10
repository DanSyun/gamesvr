/*eslint-disable block-scoped-var, id-length, no-control-regex, no-magic-numbers, no-prototype-builtins, no-redeclare, no-shadow, no-var, sort-vars*/
"use strict";

var $protobuf = require("protobufjs/minimal");

// Common aliases
var $Reader = $protobuf.Reader, $Writer = $protobuf.Writer, $util = $protobuf.util;

// Exported root namespace
var $root = $protobuf.roots["default"] || ($protobuf.roots["default"] = {});

/**
 * ENGameType enum.
 * @exports ENGameType
 * @enum {string}
 * @property {number} EN_G_Type_Lipstick=1 EN_G_Type_Lipstick value
 */
$root.ENGameType = (function() {
    var valuesById = {}, values = Object.create(valuesById);
    values[valuesById[1] = "EN_G_Type_Lipstick"] = 1;
    return values;
})();

/**
 * ENAwardType enum.
 * @exports ENAwardType
 * @enum {string}
 * @property {number} EN_Award_Gold=1 EN_Award_Gold value
 */
$root.ENAwardType = (function() {
    var valuesById = {}, values = Object.create(valuesById);
    values[valuesById[1] = "EN_Award_Gold"] = 1;
    return values;
})();

$root.PBAward = (function() {

    /**
     * Properties of a PBAward.
     * @exports IPBAward
     * @interface IPBAward
     * @property {ENAwardType|null} [type] PBAward type
     * @property {number|null} [id] PBAward id
     * @property {number|Long|null} [chargeNum] PBAward chargeNum
     * @property {number|Long|null} [givingNum] PBAward givingNum
     */

    /**
     * Constructs a new PBAward.
     * @exports PBAward
     * @classdesc Represents a PBAward.
     * @implements IPBAward
     * @constructor
     * @param {IPBAward=} [properties] Properties to set
     */
    function PBAward(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * PBAward type.
     * @member {ENAwardType} type
     * @memberof PBAward
     * @instance
     */
    PBAward.prototype.type = 1;

    /**
     * PBAward id.
     * @member {number} id
     * @memberof PBAward
     * @instance
     */
    PBAward.prototype.id = 0;

    /**
     * PBAward chargeNum.
     * @member {number|Long} chargeNum
     * @memberof PBAward
     * @instance
     */
    PBAward.prototype.chargeNum = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * PBAward givingNum.
     * @member {number|Long} givingNum
     * @memberof PBAward
     * @instance
     */
    PBAward.prototype.givingNum = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * Creates a new PBAward instance using the specified properties.
     * @function create
     * @memberof PBAward
     * @static
     * @param {IPBAward=} [properties] Properties to set
     * @returns {PBAward} PBAward instance
     */
    PBAward.create = function create(properties) {
        return new PBAward(properties);
    };

    /**
     * Encodes the specified PBAward message. Does not implicitly {@link PBAward.verify|verify} messages.
     * @function encode
     * @memberof PBAward
     * @static
     * @param {IPBAward} message PBAward message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBAward.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.type != null && message.hasOwnProperty("type"))
            writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
        if (message.id != null && message.hasOwnProperty("id"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.id);
        if (message.chargeNum != null && message.hasOwnProperty("chargeNum"))
            writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.chargeNum);
        if (message.givingNum != null && message.hasOwnProperty("givingNum"))
            writer.uint32(/* id 4, wireType 0 =*/32).uint64(message.givingNum);
        return writer;
    };

    /**
     * Encodes the specified PBAward message, length delimited. Does not implicitly {@link PBAward.verify|verify} messages.
     * @function encodeDelimited
     * @memberof PBAward
     * @static
     * @param {IPBAward} message PBAward message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBAward.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a PBAward message from the specified reader or buffer.
     * @function decode
     * @memberof PBAward
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {PBAward} PBAward
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBAward.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.PBAward();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.type = reader.int32();
                break;
            case 2:
                message.id = reader.uint32();
                break;
            case 3:
                message.chargeNum = reader.uint64();
                break;
            case 4:
                message.givingNum = reader.uint64();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a PBAward message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof PBAward
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {PBAward} PBAward
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBAward.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a PBAward message.
     * @function verify
     * @memberof PBAward
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    PBAward.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.type != null && message.hasOwnProperty("type"))
            switch (message.type) {
            default:
                return "type: enum value expected";
            case 1:
                break;
            }
        if (message.id != null && message.hasOwnProperty("id"))
            if (!$util.isInteger(message.id))
                return "id: integer expected";
        if (message.chargeNum != null && message.hasOwnProperty("chargeNum"))
            if (!$util.isInteger(message.chargeNum) && !(message.chargeNum && $util.isInteger(message.chargeNum.low) && $util.isInteger(message.chargeNum.high)))
                return "chargeNum: integer|Long expected";
        if (message.givingNum != null && message.hasOwnProperty("givingNum"))
            if (!$util.isInteger(message.givingNum) && !(message.givingNum && $util.isInteger(message.givingNum.low) && $util.isInteger(message.givingNum.high)))
                return "givingNum: integer|Long expected";
        return null;
    };

    /**
     * Creates a PBAward message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof PBAward
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {PBAward} PBAward
     */
    PBAward.fromObject = function fromObject(object) {
        if (object instanceof $root.PBAward)
            return object;
        var message = new $root.PBAward();
        switch (object.type) {
        case "EN_Award_Gold":
        case 1:
            message.type = 1;
            break;
        }
        if (object.id != null)
            message.id = object.id >>> 0;
        if (object.chargeNum != null)
            if ($util.Long)
                (message.chargeNum = $util.Long.fromValue(object.chargeNum)).unsigned = true;
            else if (typeof object.chargeNum === "string")
                message.chargeNum = parseInt(object.chargeNum, 10);
            else if (typeof object.chargeNum === "number")
                message.chargeNum = object.chargeNum;
            else if (typeof object.chargeNum === "object")
                message.chargeNum = new $util.LongBits(object.chargeNum.low >>> 0, object.chargeNum.high >>> 0).toNumber(true);
        if (object.givingNum != null)
            if ($util.Long)
                (message.givingNum = $util.Long.fromValue(object.givingNum)).unsigned = true;
            else if (typeof object.givingNum === "string")
                message.givingNum = parseInt(object.givingNum, 10);
            else if (typeof object.givingNum === "number")
                message.givingNum = object.givingNum;
            else if (typeof object.givingNum === "object")
                message.givingNum = new $util.LongBits(object.givingNum.low >>> 0, object.givingNum.high >>> 0).toNumber(true);
        return message;
    };

    /**
     * Creates a plain object from a PBAward message. Also converts values to other types if specified.
     * @function toObject
     * @memberof PBAward
     * @static
     * @param {PBAward} message PBAward
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    PBAward.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            object.type = options.enums === String ? "EN_Award_Gold" : 1;
            object.id = 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.chargeNum = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.chargeNum = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.givingNum = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.givingNum = options.longs === String ? "0" : 0;
        }
        if (message.type != null && message.hasOwnProperty("type"))
            object.type = options.enums === String ? $root.ENAwardType[message.type] : message.type;
        if (message.id != null && message.hasOwnProperty("id"))
            object.id = message.id;
        if (message.chargeNum != null && message.hasOwnProperty("chargeNum"))
            if (typeof message.chargeNum === "number")
                object.chargeNum = options.longs === String ? String(message.chargeNum) : message.chargeNum;
            else
                object.chargeNum = options.longs === String ? $util.Long.prototype.toString.call(message.chargeNum) : options.longs === Number ? new $util.LongBits(message.chargeNum.low >>> 0, message.chargeNum.high >>> 0).toNumber(true) : message.chargeNum;
        if (message.givingNum != null && message.hasOwnProperty("givingNum"))
            if (typeof message.givingNum === "number")
                object.givingNum = options.longs === String ? String(message.givingNum) : message.givingNum;
            else
                object.givingNum = options.longs === String ? $util.Long.prototype.toString.call(message.givingNum) : options.longs === Number ? new $util.LongBits(message.givingNum.low >>> 0, message.givingNum.high >>> 0).toNumber(true) : message.givingNum;
        return object;
    };

    /**
     * Converts this PBAward to JSON.
     * @function toJSON
     * @memberof PBAward
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    PBAward.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return PBAward;
})();

$root.DBDataHead = (function() {

    /**
     * Properties of a DBDataHead.
     * @exports IDBDataHead
     * @interface IDBDataHead
     * @property {number|null} [version] DBDataHead version
     */

    /**
     * Constructs a new DBDataHead.
     * @exports DBDataHead
     * @classdesc Represents a DBDataHead.
     * @implements IDBDataHead
     * @constructor
     * @param {IDBDataHead=} [properties] Properties to set
     */
    function DBDataHead(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * DBDataHead version.
     * @member {number} version
     * @memberof DBDataHead
     * @instance
     */
    DBDataHead.prototype.version = 0;

    /**
     * Creates a new DBDataHead instance using the specified properties.
     * @function create
     * @memberof DBDataHead
     * @static
     * @param {IDBDataHead=} [properties] Properties to set
     * @returns {DBDataHead} DBDataHead instance
     */
    DBDataHead.create = function create(properties) {
        return new DBDataHead(properties);
    };

    /**
     * Encodes the specified DBDataHead message. Does not implicitly {@link DBDataHead.verify|verify} messages.
     * @function encode
     * @memberof DBDataHead
     * @static
     * @param {IDBDataHead} message DBDataHead message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBDataHead.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.version != null && message.hasOwnProperty("version"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.version);
        return writer;
    };

    /**
     * Encodes the specified DBDataHead message, length delimited. Does not implicitly {@link DBDataHead.verify|verify} messages.
     * @function encodeDelimited
     * @memberof DBDataHead
     * @static
     * @param {IDBDataHead} message DBDataHead message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBDataHead.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a DBDataHead message from the specified reader or buffer.
     * @function decode
     * @memberof DBDataHead
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {DBDataHead} DBDataHead
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBDataHead.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.DBDataHead();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.version = reader.uint32();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a DBDataHead message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof DBDataHead
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {DBDataHead} DBDataHead
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBDataHead.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a DBDataHead message.
     * @function verify
     * @memberof DBDataHead
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    DBDataHead.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.version != null && message.hasOwnProperty("version"))
            if (!$util.isInteger(message.version))
                return "version: integer expected";
        return null;
    };

    /**
     * Creates a DBDataHead message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof DBDataHead
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {DBDataHead} DBDataHead
     */
    DBDataHead.fromObject = function fromObject(object) {
        if (object instanceof $root.DBDataHead)
            return object;
        var message = new $root.DBDataHead();
        if (object.version != null)
            message.version = object.version >>> 0;
        return message;
    };

    /**
     * Creates a plain object from a DBDataHead message. Also converts values to other types if specified.
     * @function toObject
     * @memberof DBDataHead
     * @static
     * @param {DBDataHead} message DBDataHead
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    DBDataHead.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults)
            object.version = 0;
        if (message.version != null && message.hasOwnProperty("version"))
            object.version = message.version;
        return object;
    };

    /**
     * Converts this DBDataHead to JSON.
     * @function toJSON
     * @memberof DBDataHead
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    DBDataHead.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return DBDataHead;
})();

$root.DBBaseInfo = (function() {

    /**
     * Properties of a DBBaseInfo.
     * @exports IDBBaseInfo
     * @interface IDBBaseInfo
     * @property {IDBDataHead|null} [head] DBBaseInfo head
     * @property {string|null} [nick] DBBaseInfo nick
     * @property {string|null} [picUrl] DBBaseInfo picUrl
     * @property {ENGameType|null} [gameType] DBBaseInfo gameType
     */

    /**
     * Constructs a new DBBaseInfo.
     * @exports DBBaseInfo
     * @classdesc Represents a DBBaseInfo.
     * @implements IDBBaseInfo
     * @constructor
     * @param {IDBBaseInfo=} [properties] Properties to set
     */
    function DBBaseInfo(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * DBBaseInfo head.
     * @member {IDBDataHead|null|undefined} head
     * @memberof DBBaseInfo
     * @instance
     */
    DBBaseInfo.prototype.head = null;

    /**
     * DBBaseInfo nick.
     * @member {string} nick
     * @memberof DBBaseInfo
     * @instance
     */
    DBBaseInfo.prototype.nick = "";

    /**
     * DBBaseInfo picUrl.
     * @member {string} picUrl
     * @memberof DBBaseInfo
     * @instance
     */
    DBBaseInfo.prototype.picUrl = "";

    /**
     * DBBaseInfo gameType.
     * @member {ENGameType} gameType
     * @memberof DBBaseInfo
     * @instance
     */
    DBBaseInfo.prototype.gameType = 1;

    /**
     * Creates a new DBBaseInfo instance using the specified properties.
     * @function create
     * @memberof DBBaseInfo
     * @static
     * @param {IDBBaseInfo=} [properties] Properties to set
     * @returns {DBBaseInfo} DBBaseInfo instance
     */
    DBBaseInfo.create = function create(properties) {
        return new DBBaseInfo(properties);
    };

    /**
     * Encodes the specified DBBaseInfo message. Does not implicitly {@link DBBaseInfo.verify|verify} messages.
     * @function encode
     * @memberof DBBaseInfo
     * @static
     * @param {IDBBaseInfo} message DBBaseInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBBaseInfo.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.head != null && message.hasOwnProperty("head"))
            $root.DBDataHead.encode(message.head, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
        if (message.nick != null && message.hasOwnProperty("nick"))
            writer.uint32(/* id 2, wireType 2 =*/18).string(message.nick);
        if (message.picUrl != null && message.hasOwnProperty("picUrl"))
            writer.uint32(/* id 3, wireType 2 =*/26).string(message.picUrl);
        if (message.gameType != null && message.hasOwnProperty("gameType"))
            writer.uint32(/* id 4, wireType 0 =*/32).int32(message.gameType);
        return writer;
    };

    /**
     * Encodes the specified DBBaseInfo message, length delimited. Does not implicitly {@link DBBaseInfo.verify|verify} messages.
     * @function encodeDelimited
     * @memberof DBBaseInfo
     * @static
     * @param {IDBBaseInfo} message DBBaseInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBBaseInfo.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a DBBaseInfo message from the specified reader or buffer.
     * @function decode
     * @memberof DBBaseInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {DBBaseInfo} DBBaseInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBBaseInfo.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.DBBaseInfo();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.head = $root.DBDataHead.decode(reader, reader.uint32());
                break;
            case 2:
                message.nick = reader.string();
                break;
            case 3:
                message.picUrl = reader.string();
                break;
            case 4:
                message.gameType = reader.int32();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a DBBaseInfo message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof DBBaseInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {DBBaseInfo} DBBaseInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBBaseInfo.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a DBBaseInfo message.
     * @function verify
     * @memberof DBBaseInfo
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    DBBaseInfo.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.head != null && message.hasOwnProperty("head")) {
            var error = $root.DBDataHead.verify(message.head);
            if (error)
                return "head." + error;
        }
        if (message.nick != null && message.hasOwnProperty("nick"))
            if (!$util.isString(message.nick))
                return "nick: string expected";
        if (message.picUrl != null && message.hasOwnProperty("picUrl"))
            if (!$util.isString(message.picUrl))
                return "picUrl: string expected";
        if (message.gameType != null && message.hasOwnProperty("gameType"))
            switch (message.gameType) {
            default:
                return "gameType: enum value expected";
            case 1:
                break;
            }
        return null;
    };

    /**
     * Creates a DBBaseInfo message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof DBBaseInfo
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {DBBaseInfo} DBBaseInfo
     */
    DBBaseInfo.fromObject = function fromObject(object) {
        if (object instanceof $root.DBBaseInfo)
            return object;
        var message = new $root.DBBaseInfo();
        if (object.head != null) {
            if (typeof object.head !== "object")
                throw TypeError(".DBBaseInfo.head: object expected");
            message.head = $root.DBDataHead.fromObject(object.head);
        }
        if (object.nick != null)
            message.nick = String(object.nick);
        if (object.picUrl != null)
            message.picUrl = String(object.picUrl);
        switch (object.gameType) {
        case "EN_G_Type_Lipstick":
        case 1:
            message.gameType = 1;
            break;
        }
        return message;
    };

    /**
     * Creates a plain object from a DBBaseInfo message. Also converts values to other types if specified.
     * @function toObject
     * @memberof DBBaseInfo
     * @static
     * @param {DBBaseInfo} message DBBaseInfo
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    DBBaseInfo.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            object.head = null;
            object.nick = "";
            object.picUrl = "";
            object.gameType = options.enums === String ? "EN_G_Type_Lipstick" : 1;
        }
        if (message.head != null && message.hasOwnProperty("head"))
            object.head = $root.DBDataHead.toObject(message.head, options);
        if (message.nick != null && message.hasOwnProperty("nick"))
            object.nick = message.nick;
        if (message.picUrl != null && message.hasOwnProperty("picUrl"))
            object.picUrl = message.picUrl;
        if (message.gameType != null && message.hasOwnProperty("gameType"))
            object.gameType = options.enums === String ? $root.ENGameType[message.gameType] : message.gameType;
        return object;
    };

    /**
     * Converts this DBBaseInfo to JSON.
     * @function toJSON
     * @memberof DBBaseInfo
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    DBBaseInfo.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return DBBaseInfo;
})();

$root.DBBuyIn = (function() {

    /**
     * Properties of a DBBuyIn.
     * @exports IDBBuyIn
     * @interface IDBBuyIn
     * @property {number|Long|null} [machineId] DBBuyIn machineId
     * @property {number|Long|null} [num] DBBuyIn num
     * @property {number|null} [updateTime] DBBuyIn updateTime
     */

    /**
     * Constructs a new DBBuyIn.
     * @exports DBBuyIn
     * @classdesc Represents a DBBuyIn.
     * @implements IDBBuyIn
     * @constructor
     * @param {IDBBuyIn=} [properties] Properties to set
     */
    function DBBuyIn(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * DBBuyIn machineId.
     * @member {number|Long} machineId
     * @memberof DBBuyIn
     * @instance
     */
    DBBuyIn.prototype.machineId = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * DBBuyIn num.
     * @member {number|Long} num
     * @memberof DBBuyIn
     * @instance
     */
    DBBuyIn.prototype.num = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * DBBuyIn updateTime.
     * @member {number} updateTime
     * @memberof DBBuyIn
     * @instance
     */
    DBBuyIn.prototype.updateTime = 0;

    /**
     * Creates a new DBBuyIn instance using the specified properties.
     * @function create
     * @memberof DBBuyIn
     * @static
     * @param {IDBBuyIn=} [properties] Properties to set
     * @returns {DBBuyIn} DBBuyIn instance
     */
    DBBuyIn.create = function create(properties) {
        return new DBBuyIn(properties);
    };

    /**
     * Encodes the specified DBBuyIn message. Does not implicitly {@link DBBuyIn.verify|verify} messages.
     * @function encode
     * @memberof DBBuyIn
     * @static
     * @param {IDBBuyIn} message DBBuyIn message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBBuyIn.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.machineId);
        if (message.num != null && message.hasOwnProperty("num"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.num);
        if (message.updateTime != null && message.hasOwnProperty("updateTime"))
            writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.updateTime);
        return writer;
    };

    /**
     * Encodes the specified DBBuyIn message, length delimited. Does not implicitly {@link DBBuyIn.verify|verify} messages.
     * @function encodeDelimited
     * @memberof DBBuyIn
     * @static
     * @param {IDBBuyIn} message DBBuyIn message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBBuyIn.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a DBBuyIn message from the specified reader or buffer.
     * @function decode
     * @memberof DBBuyIn
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {DBBuyIn} DBBuyIn
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBBuyIn.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.DBBuyIn();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.machineId = reader.uint64();
                break;
            case 2:
                message.num = reader.uint64();
                break;
            case 3:
                message.updateTime = reader.uint32();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a DBBuyIn message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof DBBuyIn
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {DBBuyIn} DBBuyIn
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBBuyIn.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a DBBuyIn message.
     * @function verify
     * @memberof DBBuyIn
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    DBBuyIn.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            if (!$util.isInteger(message.machineId) && !(message.machineId && $util.isInteger(message.machineId.low) && $util.isInteger(message.machineId.high)))
                return "machineId: integer|Long expected";
        if (message.num != null && message.hasOwnProperty("num"))
            if (!$util.isInteger(message.num) && !(message.num && $util.isInteger(message.num.low) && $util.isInteger(message.num.high)))
                return "num: integer|Long expected";
        if (message.updateTime != null && message.hasOwnProperty("updateTime"))
            if (!$util.isInteger(message.updateTime))
                return "updateTime: integer expected";
        return null;
    };

    /**
     * Creates a DBBuyIn message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof DBBuyIn
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {DBBuyIn} DBBuyIn
     */
    DBBuyIn.fromObject = function fromObject(object) {
        if (object instanceof $root.DBBuyIn)
            return object;
        var message = new $root.DBBuyIn();
        if (object.machineId != null)
            if ($util.Long)
                (message.machineId = $util.Long.fromValue(object.machineId)).unsigned = true;
            else if (typeof object.machineId === "string")
                message.machineId = parseInt(object.machineId, 10);
            else if (typeof object.machineId === "number")
                message.machineId = object.machineId;
            else if (typeof object.machineId === "object")
                message.machineId = new $util.LongBits(object.machineId.low >>> 0, object.machineId.high >>> 0).toNumber(true);
        if (object.num != null)
            if ($util.Long)
                (message.num = $util.Long.fromValue(object.num)).unsigned = true;
            else if (typeof object.num === "string")
                message.num = parseInt(object.num, 10);
            else if (typeof object.num === "number")
                message.num = object.num;
            else if (typeof object.num === "object")
                message.num = new $util.LongBits(object.num.low >>> 0, object.num.high >>> 0).toNumber(true);
        if (object.updateTime != null)
            message.updateTime = object.updateTime >>> 0;
        return message;
    };

    /**
     * Creates a plain object from a DBBuyIn message. Also converts values to other types if specified.
     * @function toObject
     * @memberof DBBuyIn
     * @static
     * @param {DBBuyIn} message DBBuyIn
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    DBBuyIn.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.machineId = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.machineId = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.num = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.num = options.longs === String ? "0" : 0;
            object.updateTime = 0;
        }
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            if (typeof message.machineId === "number")
                object.machineId = options.longs === String ? String(message.machineId) : message.machineId;
            else
                object.machineId = options.longs === String ? $util.Long.prototype.toString.call(message.machineId) : options.longs === Number ? new $util.LongBits(message.machineId.low >>> 0, message.machineId.high >>> 0).toNumber(true) : message.machineId;
        if (message.num != null && message.hasOwnProperty("num"))
            if (typeof message.num === "number")
                object.num = options.longs === String ? String(message.num) : message.num;
            else
                object.num = options.longs === String ? $util.Long.prototype.toString.call(message.num) : options.longs === Number ? new $util.LongBits(message.num.low >>> 0, message.num.high >>> 0).toNumber(true) : message.num;
        if (message.updateTime != null && message.hasOwnProperty("updateTime"))
            object.updateTime = message.updateTime;
        return object;
    };

    /**
     * Converts this DBBuyIn to JSON.
     * @function toJSON
     * @memberof DBBuyIn
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    DBBuyIn.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return DBBuyIn;
})();

$root.DBGoldInfo = (function() {

    /**
     * Properties of a DBGoldInfo.
     * @exports IDBGoldInfo
     * @interface IDBGoldInfo
     * @property {number|Long|null} [gold] DBGoldInfo gold
     * @property {Array.<IDBBuyIn>|null} [buyIns] DBGoldInfo buyIns
     * @property {number|Long|null} [totalCharge] DBGoldInfo totalCharge
     * @property {number|Long|null} [totalGiving] DBGoldInfo totalGiving
     * @property {number|Long|null} [totalConsume] DBGoldInfo totalConsume
     */

    /**
     * Constructs a new DBGoldInfo.
     * @exports DBGoldInfo
     * @classdesc Represents a DBGoldInfo.
     * @implements IDBGoldInfo
     * @constructor
     * @param {IDBGoldInfo=} [properties] Properties to set
     */
    function DBGoldInfo(properties) {
        this.buyIns = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * DBGoldInfo gold.
     * @member {number|Long} gold
     * @memberof DBGoldInfo
     * @instance
     */
    DBGoldInfo.prototype.gold = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * DBGoldInfo buyIns.
     * @member {Array.<IDBBuyIn>} buyIns
     * @memberof DBGoldInfo
     * @instance
     */
    DBGoldInfo.prototype.buyIns = $util.emptyArray;

    /**
     * DBGoldInfo totalCharge.
     * @member {number|Long} totalCharge
     * @memberof DBGoldInfo
     * @instance
     */
    DBGoldInfo.prototype.totalCharge = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * DBGoldInfo totalGiving.
     * @member {number|Long} totalGiving
     * @memberof DBGoldInfo
     * @instance
     */
    DBGoldInfo.prototype.totalGiving = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * DBGoldInfo totalConsume.
     * @member {number|Long} totalConsume
     * @memberof DBGoldInfo
     * @instance
     */
    DBGoldInfo.prototype.totalConsume = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * Creates a new DBGoldInfo instance using the specified properties.
     * @function create
     * @memberof DBGoldInfo
     * @static
     * @param {IDBGoldInfo=} [properties] Properties to set
     * @returns {DBGoldInfo} DBGoldInfo instance
     */
    DBGoldInfo.create = function create(properties) {
        return new DBGoldInfo(properties);
    };

    /**
     * Encodes the specified DBGoldInfo message. Does not implicitly {@link DBGoldInfo.verify|verify} messages.
     * @function encode
     * @memberof DBGoldInfo
     * @static
     * @param {IDBGoldInfo} message DBGoldInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBGoldInfo.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.gold != null && message.hasOwnProperty("gold"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.gold);
        if (message.buyIns != null && message.buyIns.length)
            for (var i = 0; i < message.buyIns.length; ++i)
                $root.DBBuyIn.encode(message.buyIns[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
        if (message.totalCharge != null && message.hasOwnProperty("totalCharge"))
            writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.totalCharge);
        if (message.totalGiving != null && message.hasOwnProperty("totalGiving"))
            writer.uint32(/* id 4, wireType 0 =*/32).uint64(message.totalGiving);
        if (message.totalConsume != null && message.hasOwnProperty("totalConsume"))
            writer.uint32(/* id 5, wireType 0 =*/40).uint64(message.totalConsume);
        return writer;
    };

    /**
     * Encodes the specified DBGoldInfo message, length delimited. Does not implicitly {@link DBGoldInfo.verify|verify} messages.
     * @function encodeDelimited
     * @memberof DBGoldInfo
     * @static
     * @param {IDBGoldInfo} message DBGoldInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBGoldInfo.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a DBGoldInfo message from the specified reader or buffer.
     * @function decode
     * @memberof DBGoldInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {DBGoldInfo} DBGoldInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBGoldInfo.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.DBGoldInfo();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.gold = reader.uint64();
                break;
            case 2:
                if (!(message.buyIns && message.buyIns.length))
                    message.buyIns = [];
                message.buyIns.push($root.DBBuyIn.decode(reader, reader.uint32()));
                break;
            case 3:
                message.totalCharge = reader.uint64();
                break;
            case 4:
                message.totalGiving = reader.uint64();
                break;
            case 5:
                message.totalConsume = reader.uint64();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a DBGoldInfo message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof DBGoldInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {DBGoldInfo} DBGoldInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBGoldInfo.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a DBGoldInfo message.
     * @function verify
     * @memberof DBGoldInfo
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    DBGoldInfo.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.gold != null && message.hasOwnProperty("gold"))
            if (!$util.isInteger(message.gold) && !(message.gold && $util.isInteger(message.gold.low) && $util.isInteger(message.gold.high)))
                return "gold: integer|Long expected";
        if (message.buyIns != null && message.hasOwnProperty("buyIns")) {
            if (!Array.isArray(message.buyIns))
                return "buyIns: array expected";
            for (var i = 0; i < message.buyIns.length; ++i) {
                var error = $root.DBBuyIn.verify(message.buyIns[i]);
                if (error)
                    return "buyIns." + error;
            }
        }
        if (message.totalCharge != null && message.hasOwnProperty("totalCharge"))
            if (!$util.isInteger(message.totalCharge) && !(message.totalCharge && $util.isInteger(message.totalCharge.low) && $util.isInteger(message.totalCharge.high)))
                return "totalCharge: integer|Long expected";
        if (message.totalGiving != null && message.hasOwnProperty("totalGiving"))
            if (!$util.isInteger(message.totalGiving) && !(message.totalGiving && $util.isInteger(message.totalGiving.low) && $util.isInteger(message.totalGiving.high)))
                return "totalGiving: integer|Long expected";
        if (message.totalConsume != null && message.hasOwnProperty("totalConsume"))
            if (!$util.isInteger(message.totalConsume) && !(message.totalConsume && $util.isInteger(message.totalConsume.low) && $util.isInteger(message.totalConsume.high)))
                return "totalConsume: integer|Long expected";
        return null;
    };

    /**
     * Creates a DBGoldInfo message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof DBGoldInfo
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {DBGoldInfo} DBGoldInfo
     */
    DBGoldInfo.fromObject = function fromObject(object) {
        if (object instanceof $root.DBGoldInfo)
            return object;
        var message = new $root.DBGoldInfo();
        if (object.gold != null)
            if ($util.Long)
                (message.gold = $util.Long.fromValue(object.gold)).unsigned = true;
            else if (typeof object.gold === "string")
                message.gold = parseInt(object.gold, 10);
            else if (typeof object.gold === "number")
                message.gold = object.gold;
            else if (typeof object.gold === "object")
                message.gold = new $util.LongBits(object.gold.low >>> 0, object.gold.high >>> 0).toNumber(true);
        if (object.buyIns) {
            if (!Array.isArray(object.buyIns))
                throw TypeError(".DBGoldInfo.buyIns: array expected");
            message.buyIns = [];
            for (var i = 0; i < object.buyIns.length; ++i) {
                if (typeof object.buyIns[i] !== "object")
                    throw TypeError(".DBGoldInfo.buyIns: object expected");
                message.buyIns[i] = $root.DBBuyIn.fromObject(object.buyIns[i]);
            }
        }
        if (object.totalCharge != null)
            if ($util.Long)
                (message.totalCharge = $util.Long.fromValue(object.totalCharge)).unsigned = true;
            else if (typeof object.totalCharge === "string")
                message.totalCharge = parseInt(object.totalCharge, 10);
            else if (typeof object.totalCharge === "number")
                message.totalCharge = object.totalCharge;
            else if (typeof object.totalCharge === "object")
                message.totalCharge = new $util.LongBits(object.totalCharge.low >>> 0, object.totalCharge.high >>> 0).toNumber(true);
        if (object.totalGiving != null)
            if ($util.Long)
                (message.totalGiving = $util.Long.fromValue(object.totalGiving)).unsigned = true;
            else if (typeof object.totalGiving === "string")
                message.totalGiving = parseInt(object.totalGiving, 10);
            else if (typeof object.totalGiving === "number")
                message.totalGiving = object.totalGiving;
            else if (typeof object.totalGiving === "object")
                message.totalGiving = new $util.LongBits(object.totalGiving.low >>> 0, object.totalGiving.high >>> 0).toNumber(true);
        if (object.totalConsume != null)
            if ($util.Long)
                (message.totalConsume = $util.Long.fromValue(object.totalConsume)).unsigned = true;
            else if (typeof object.totalConsume === "string")
                message.totalConsume = parseInt(object.totalConsume, 10);
            else if (typeof object.totalConsume === "number")
                message.totalConsume = object.totalConsume;
            else if (typeof object.totalConsume === "object")
                message.totalConsume = new $util.LongBits(object.totalConsume.low >>> 0, object.totalConsume.high >>> 0).toNumber(true);
        return message;
    };

    /**
     * Creates a plain object from a DBGoldInfo message. Also converts values to other types if specified.
     * @function toObject
     * @memberof DBGoldInfo
     * @static
     * @param {DBGoldInfo} message DBGoldInfo
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    DBGoldInfo.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.buyIns = [];
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.gold = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.gold = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.totalCharge = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.totalCharge = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.totalGiving = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.totalGiving = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.totalConsume = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.totalConsume = options.longs === String ? "0" : 0;
        }
        if (message.gold != null && message.hasOwnProperty("gold"))
            if (typeof message.gold === "number")
                object.gold = options.longs === String ? String(message.gold) : message.gold;
            else
                object.gold = options.longs === String ? $util.Long.prototype.toString.call(message.gold) : options.longs === Number ? new $util.LongBits(message.gold.low >>> 0, message.gold.high >>> 0).toNumber(true) : message.gold;
        if (message.buyIns && message.buyIns.length) {
            object.buyIns = [];
            for (var j = 0; j < message.buyIns.length; ++j)
                object.buyIns[j] = $root.DBBuyIn.toObject(message.buyIns[j], options);
        }
        if (message.totalCharge != null && message.hasOwnProperty("totalCharge"))
            if (typeof message.totalCharge === "number")
                object.totalCharge = options.longs === String ? String(message.totalCharge) : message.totalCharge;
            else
                object.totalCharge = options.longs === String ? $util.Long.prototype.toString.call(message.totalCharge) : options.longs === Number ? new $util.LongBits(message.totalCharge.low >>> 0, message.totalCharge.high >>> 0).toNumber(true) : message.totalCharge;
        if (message.totalGiving != null && message.hasOwnProperty("totalGiving"))
            if (typeof message.totalGiving === "number")
                object.totalGiving = options.longs === String ? String(message.totalGiving) : message.totalGiving;
            else
                object.totalGiving = options.longs === String ? $util.Long.prototype.toString.call(message.totalGiving) : options.longs === Number ? new $util.LongBits(message.totalGiving.low >>> 0, message.totalGiving.high >>> 0).toNumber(true) : message.totalGiving;
        if (message.totalConsume != null && message.hasOwnProperty("totalConsume"))
            if (typeof message.totalConsume === "number")
                object.totalConsume = options.longs === String ? String(message.totalConsume) : message.totalConsume;
            else
                object.totalConsume = options.longs === String ? $util.Long.prototype.toString.call(message.totalConsume) : options.longs === Number ? new $util.LongBits(message.totalConsume.low >>> 0, message.totalConsume.high >>> 0).toNumber(true) : message.totalConsume;
        return object;
    };

    /**
     * Converts this DBGoldInfo to JSON.
     * @function toJSON
     * @memberof DBGoldInfo
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    DBGoldInfo.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return DBGoldInfo;
})();

$root.DBMBaseInfo = (function() {

    /**
     * Properties of a DBMBaseInfo.
     * @exports IDBMBaseInfo
     * @interface IDBMBaseInfo
     * @property {IDBDataHead|null} [head] DBMBaseInfo head
     * @property {number|null} [svrId] DBMBaseInfo svrId
     * @property {number|null} [gameId] DBMBaseInfo gameId
     * @property {ENGameType|null} [gameType] DBMBaseInfo gameType
     * @property {number|Long|null} [agentId] DBMBaseInfo agentId
     */

    /**
     * Constructs a new DBMBaseInfo.
     * @exports DBMBaseInfo
     * @classdesc Represents a DBMBaseInfo.
     * @implements IDBMBaseInfo
     * @constructor
     * @param {IDBMBaseInfo=} [properties] Properties to set
     */
    function DBMBaseInfo(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * DBMBaseInfo head.
     * @member {IDBDataHead|null|undefined} head
     * @memberof DBMBaseInfo
     * @instance
     */
    DBMBaseInfo.prototype.head = null;

    /**
     * DBMBaseInfo svrId.
     * @member {number} svrId
     * @memberof DBMBaseInfo
     * @instance
     */
    DBMBaseInfo.prototype.svrId = 0;

    /**
     * DBMBaseInfo gameId.
     * @member {number} gameId
     * @memberof DBMBaseInfo
     * @instance
     */
    DBMBaseInfo.prototype.gameId = 0;

    /**
     * DBMBaseInfo gameType.
     * @member {ENGameType} gameType
     * @memberof DBMBaseInfo
     * @instance
     */
    DBMBaseInfo.prototype.gameType = 1;

    /**
     * DBMBaseInfo agentId.
     * @member {number|Long} agentId
     * @memberof DBMBaseInfo
     * @instance
     */
    DBMBaseInfo.prototype.agentId = $util.Long ? $util.Long.fromBits(88888,0,true) : 88888;

    /**
     * Creates a new DBMBaseInfo instance using the specified properties.
     * @function create
     * @memberof DBMBaseInfo
     * @static
     * @param {IDBMBaseInfo=} [properties] Properties to set
     * @returns {DBMBaseInfo} DBMBaseInfo instance
     */
    DBMBaseInfo.create = function create(properties) {
        return new DBMBaseInfo(properties);
    };

    /**
     * Encodes the specified DBMBaseInfo message. Does not implicitly {@link DBMBaseInfo.verify|verify} messages.
     * @function encode
     * @memberof DBMBaseInfo
     * @static
     * @param {IDBMBaseInfo} message DBMBaseInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBMBaseInfo.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.head != null && message.hasOwnProperty("head"))
            $root.DBDataHead.encode(message.head, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
        if (message.svrId != null && message.hasOwnProperty("svrId"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.svrId);
        if (message.gameId != null && message.hasOwnProperty("gameId"))
            writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.gameId);
        if (message.gameType != null && message.hasOwnProperty("gameType"))
            writer.uint32(/* id 10, wireType 0 =*/80).int32(message.gameType);
        if (message.agentId != null && message.hasOwnProperty("agentId"))
            writer.uint32(/* id 11, wireType 0 =*/88).uint64(message.agentId);
        return writer;
    };

    /**
     * Encodes the specified DBMBaseInfo message, length delimited. Does not implicitly {@link DBMBaseInfo.verify|verify} messages.
     * @function encodeDelimited
     * @memberof DBMBaseInfo
     * @static
     * @param {IDBMBaseInfo} message DBMBaseInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBMBaseInfo.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a DBMBaseInfo message from the specified reader or buffer.
     * @function decode
     * @memberof DBMBaseInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {DBMBaseInfo} DBMBaseInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBMBaseInfo.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.DBMBaseInfo();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.head = $root.DBDataHead.decode(reader, reader.uint32());
                break;
            case 2:
                message.svrId = reader.uint32();
                break;
            case 3:
                message.gameId = reader.uint32();
                break;
            case 10:
                message.gameType = reader.int32();
                break;
            case 11:
                message.agentId = reader.uint64();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a DBMBaseInfo message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof DBMBaseInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {DBMBaseInfo} DBMBaseInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBMBaseInfo.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a DBMBaseInfo message.
     * @function verify
     * @memberof DBMBaseInfo
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    DBMBaseInfo.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.head != null && message.hasOwnProperty("head")) {
            var error = $root.DBDataHead.verify(message.head);
            if (error)
                return "head." + error;
        }
        if (message.svrId != null && message.hasOwnProperty("svrId"))
            if (!$util.isInteger(message.svrId))
                return "svrId: integer expected";
        if (message.gameId != null && message.hasOwnProperty("gameId"))
            if (!$util.isInteger(message.gameId))
                return "gameId: integer expected";
        if (message.gameType != null && message.hasOwnProperty("gameType"))
            switch (message.gameType) {
            default:
                return "gameType: enum value expected";
            case 1:
                break;
            }
        if (message.agentId != null && message.hasOwnProperty("agentId"))
            if (!$util.isInteger(message.agentId) && !(message.agentId && $util.isInteger(message.agentId.low) && $util.isInteger(message.agentId.high)))
                return "agentId: integer|Long expected";
        return null;
    };

    /**
     * Creates a DBMBaseInfo message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof DBMBaseInfo
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {DBMBaseInfo} DBMBaseInfo
     */
    DBMBaseInfo.fromObject = function fromObject(object) {
        if (object instanceof $root.DBMBaseInfo)
            return object;
        var message = new $root.DBMBaseInfo();
        if (object.head != null) {
            if (typeof object.head !== "object")
                throw TypeError(".DBMBaseInfo.head: object expected");
            message.head = $root.DBDataHead.fromObject(object.head);
        }
        if (object.svrId != null)
            message.svrId = object.svrId >>> 0;
        if (object.gameId != null)
            message.gameId = object.gameId >>> 0;
        switch (object.gameType) {
        case "EN_G_Type_Lipstick":
        case 1:
            message.gameType = 1;
            break;
        }
        if (object.agentId != null)
            if ($util.Long)
                (message.agentId = $util.Long.fromValue(object.agentId)).unsigned = true;
            else if (typeof object.agentId === "string")
                message.agentId = parseInt(object.agentId, 10);
            else if (typeof object.agentId === "number")
                message.agentId = object.agentId;
            else if (typeof object.agentId === "object")
                message.agentId = new $util.LongBits(object.agentId.low >>> 0, object.agentId.high >>> 0).toNumber(true);
        return message;
    };

    /**
     * Creates a plain object from a DBMBaseInfo message. Also converts values to other types if specified.
     * @function toObject
     * @memberof DBMBaseInfo
     * @static
     * @param {DBMBaseInfo} message DBMBaseInfo
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    DBMBaseInfo.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            object.head = null;
            object.svrId = 0;
            object.gameId = 0;
            object.gameType = options.enums === String ? "EN_G_Type_Lipstick" : 1;
            if ($util.Long) {
                var long = new $util.Long(88888, 0, true);
                object.agentId = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.agentId = options.longs === String ? "88888" : 88888;
        }
        if (message.head != null && message.hasOwnProperty("head"))
            object.head = $root.DBDataHead.toObject(message.head, options);
        if (message.svrId != null && message.hasOwnProperty("svrId"))
            object.svrId = message.svrId;
        if (message.gameId != null && message.hasOwnProperty("gameId"))
            object.gameId = message.gameId;
        if (message.gameType != null && message.hasOwnProperty("gameType"))
            object.gameType = options.enums === String ? $root.ENGameType[message.gameType] : message.gameType;
        if (message.agentId != null && message.hasOwnProperty("agentId"))
            if (typeof message.agentId === "number")
                object.agentId = options.longs === String ? String(message.agentId) : message.agentId;
            else
                object.agentId = options.longs === String ? $util.Long.prototype.toString.call(message.agentId) : options.longs === Number ? new $util.LongBits(message.agentId.low >>> 0, message.agentId.high >>> 0).toNumber(true) : message.agentId;
        return object;
    };

    /**
     * Converts this DBMBaseInfo to JSON.
     * @function toJSON
     * @memberof DBMBaseInfo
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    DBMBaseInfo.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return DBMBaseInfo;
})();

$root.DBMLipstickInfo = (function() {

    /**
     * Properties of a DBMLipstickInfo.
     * @exports IDBMLipstickInfo
     * @interface IDBMLipstickInfo
     * @property {IDBDataHead|null} [head] DBMLipstickInfo head
     * @property {number|Long|null} [cost] DBMLipstickInfo cost
     * @property {Array.<number>|null} [prizeIds] DBMLipstickInfo prizeIds
     */

    /**
     * Constructs a new DBMLipstickInfo.
     * @exports DBMLipstickInfo
     * @classdesc Represents a DBMLipstickInfo.
     * @implements IDBMLipstickInfo
     * @constructor
     * @param {IDBMLipstickInfo=} [properties] Properties to set
     */
    function DBMLipstickInfo(properties) {
        this.prizeIds = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * DBMLipstickInfo head.
     * @member {IDBDataHead|null|undefined} head
     * @memberof DBMLipstickInfo
     * @instance
     */
    DBMLipstickInfo.prototype.head = null;

    /**
     * DBMLipstickInfo cost.
     * @member {number|Long} cost
     * @memberof DBMLipstickInfo
     * @instance
     */
    DBMLipstickInfo.prototype.cost = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * DBMLipstickInfo prizeIds.
     * @member {Array.<number>} prizeIds
     * @memberof DBMLipstickInfo
     * @instance
     */
    DBMLipstickInfo.prototype.prizeIds = $util.emptyArray;

    /**
     * Creates a new DBMLipstickInfo instance using the specified properties.
     * @function create
     * @memberof DBMLipstickInfo
     * @static
     * @param {IDBMLipstickInfo=} [properties] Properties to set
     * @returns {DBMLipstickInfo} DBMLipstickInfo instance
     */
    DBMLipstickInfo.create = function create(properties) {
        return new DBMLipstickInfo(properties);
    };

    /**
     * Encodes the specified DBMLipstickInfo message. Does not implicitly {@link DBMLipstickInfo.verify|verify} messages.
     * @function encode
     * @memberof DBMLipstickInfo
     * @static
     * @param {IDBMLipstickInfo} message DBMLipstickInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBMLipstickInfo.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.head != null && message.hasOwnProperty("head"))
            $root.DBDataHead.encode(message.head, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
        if (message.cost != null && message.hasOwnProperty("cost"))
            writer.uint32(/* id 10, wireType 0 =*/80).uint64(message.cost);
        if (message.prizeIds != null && message.prizeIds.length)
            for (var i = 0; i < message.prizeIds.length; ++i)
                writer.uint32(/* id 11, wireType 0 =*/88).uint32(message.prizeIds[i]);
        return writer;
    };

    /**
     * Encodes the specified DBMLipstickInfo message, length delimited. Does not implicitly {@link DBMLipstickInfo.verify|verify} messages.
     * @function encodeDelimited
     * @memberof DBMLipstickInfo
     * @static
     * @param {IDBMLipstickInfo} message DBMLipstickInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    DBMLipstickInfo.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a DBMLipstickInfo message from the specified reader or buffer.
     * @function decode
     * @memberof DBMLipstickInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {DBMLipstickInfo} DBMLipstickInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBMLipstickInfo.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.DBMLipstickInfo();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.head = $root.DBDataHead.decode(reader, reader.uint32());
                break;
            case 10:
                message.cost = reader.uint64();
                break;
            case 11:
                if (!(message.prizeIds && message.prizeIds.length))
                    message.prizeIds = [];
                if ((tag & 7) === 2) {
                    var end2 = reader.uint32() + reader.pos;
                    while (reader.pos < end2)
                        message.prizeIds.push(reader.uint32());
                } else
                    message.prizeIds.push(reader.uint32());
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a DBMLipstickInfo message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof DBMLipstickInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {DBMLipstickInfo} DBMLipstickInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    DBMLipstickInfo.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a DBMLipstickInfo message.
     * @function verify
     * @memberof DBMLipstickInfo
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    DBMLipstickInfo.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.head != null && message.hasOwnProperty("head")) {
            var error = $root.DBDataHead.verify(message.head);
            if (error)
                return "head." + error;
        }
        if (message.cost != null && message.hasOwnProperty("cost"))
            if (!$util.isInteger(message.cost) && !(message.cost && $util.isInteger(message.cost.low) && $util.isInteger(message.cost.high)))
                return "cost: integer|Long expected";
        if (message.prizeIds != null && message.hasOwnProperty("prizeIds")) {
            if (!Array.isArray(message.prizeIds))
                return "prizeIds: array expected";
            for (var i = 0; i < message.prizeIds.length; ++i)
                if (!$util.isInteger(message.prizeIds[i]))
                    return "prizeIds: integer[] expected";
        }
        return null;
    };

    /**
     * Creates a DBMLipstickInfo message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof DBMLipstickInfo
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {DBMLipstickInfo} DBMLipstickInfo
     */
    DBMLipstickInfo.fromObject = function fromObject(object) {
        if (object instanceof $root.DBMLipstickInfo)
            return object;
        var message = new $root.DBMLipstickInfo();
        if (object.head != null) {
            if (typeof object.head !== "object")
                throw TypeError(".DBMLipstickInfo.head: object expected");
            message.head = $root.DBDataHead.fromObject(object.head);
        }
        if (object.cost != null)
            if ($util.Long)
                (message.cost = $util.Long.fromValue(object.cost)).unsigned = true;
            else if (typeof object.cost === "string")
                message.cost = parseInt(object.cost, 10);
            else if (typeof object.cost === "number")
                message.cost = object.cost;
            else if (typeof object.cost === "object")
                message.cost = new $util.LongBits(object.cost.low >>> 0, object.cost.high >>> 0).toNumber(true);
        if (object.prizeIds) {
            if (!Array.isArray(object.prizeIds))
                throw TypeError(".DBMLipstickInfo.prizeIds: array expected");
            message.prizeIds = [];
            for (var i = 0; i < object.prizeIds.length; ++i)
                message.prizeIds[i] = object.prizeIds[i] >>> 0;
        }
        return message;
    };

    /**
     * Creates a plain object from a DBMLipstickInfo message. Also converts values to other types if specified.
     * @function toObject
     * @memberof DBMLipstickInfo
     * @static
     * @param {DBMLipstickInfo} message DBMLipstickInfo
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    DBMLipstickInfo.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.prizeIds = [];
        if (options.defaults) {
            object.head = null;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.cost = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.cost = options.longs === String ? "0" : 0;
        }
        if (message.head != null && message.hasOwnProperty("head"))
            object.head = $root.DBDataHead.toObject(message.head, options);
        if (message.cost != null && message.hasOwnProperty("cost"))
            if (typeof message.cost === "number")
                object.cost = options.longs === String ? String(message.cost) : message.cost;
            else
                object.cost = options.longs === String ? $util.Long.prototype.toString.call(message.cost) : options.longs === Number ? new $util.LongBits(message.cost.low >>> 0, message.cost.high >>> 0).toNumber(true) : message.cost;
        if (message.prizeIds && message.prizeIds.length) {
            object.prizeIds = [];
            for (var j = 0; j < message.prizeIds.length; ++j)
                object.prizeIds[j] = message.prizeIds[j];
        }
        return object;
    };

    /**
     * Converts this DBMLipstickInfo to JSON.
     * @function toJSON
     * @memberof DBMLipstickInfo
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    DBMLipstickInfo.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return DBMLipstickInfo;
})();

$root.PBDBData = (function() {

    /**
     * Properties of a PBDBData.
     * @exports IPBDBData
     * @interface IPBDBData
     * @property {number|Long|null} [uid] PBDBData uid
     * @property {IDBBaseInfo|null} [baseInfo] PBDBData baseInfo
     * @property {IDBGoldInfo|null} [goldInfo] PBDBData goldInfo
     * @property {IDBMBaseInfo|null} [mBaseInfo] PBDBData mBaseInfo
     * @property {IDBMLipstickInfo|null} [mLipstickInfo] PBDBData mLipstickInfo
     */

    /**
     * Constructs a new PBDBData.
     * @exports PBDBData
     * @classdesc Represents a PBDBData.
     * @implements IPBDBData
     * @constructor
     * @param {IPBDBData=} [properties] Properties to set
     */
    function PBDBData(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * PBDBData uid.
     * @member {number|Long} uid
     * @memberof PBDBData
     * @instance
     */
    PBDBData.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * PBDBData baseInfo.
     * @member {IDBBaseInfo|null|undefined} baseInfo
     * @memberof PBDBData
     * @instance
     */
    PBDBData.prototype.baseInfo = null;

    /**
     * PBDBData goldInfo.
     * @member {IDBGoldInfo|null|undefined} goldInfo
     * @memberof PBDBData
     * @instance
     */
    PBDBData.prototype.goldInfo = null;

    /**
     * PBDBData mBaseInfo.
     * @member {IDBMBaseInfo|null|undefined} mBaseInfo
     * @memberof PBDBData
     * @instance
     */
    PBDBData.prototype.mBaseInfo = null;

    /**
     * PBDBData mLipstickInfo.
     * @member {IDBMLipstickInfo|null|undefined} mLipstickInfo
     * @memberof PBDBData
     * @instance
     */
    PBDBData.prototype.mLipstickInfo = null;

    /**
     * Creates a new PBDBData instance using the specified properties.
     * @function create
     * @memberof PBDBData
     * @static
     * @param {IPBDBData=} [properties] Properties to set
     * @returns {PBDBData} PBDBData instance
     */
    PBDBData.create = function create(properties) {
        return new PBDBData(properties);
    };

    /**
     * Encodes the specified PBDBData message. Does not implicitly {@link PBDBData.verify|verify} messages.
     * @function encode
     * @memberof PBDBData
     * @static
     * @param {IPBDBData} message PBDBData message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBDBData.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.baseInfo != null && message.hasOwnProperty("baseInfo"))
            $root.DBBaseInfo.encode(message.baseInfo, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
        if (message.goldInfo != null && message.hasOwnProperty("goldInfo"))
            $root.DBGoldInfo.encode(message.goldInfo, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
        if (message.mBaseInfo != null && message.hasOwnProperty("mBaseInfo"))
            $root.DBMBaseInfo.encode(message.mBaseInfo, writer.uint32(/* id 100, wireType 2 =*/802).fork()).ldelim();
        if (message.mLipstickInfo != null && message.hasOwnProperty("mLipstickInfo"))
            $root.DBMLipstickInfo.encode(message.mLipstickInfo, writer.uint32(/* id 101, wireType 2 =*/810).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified PBDBData message, length delimited. Does not implicitly {@link PBDBData.verify|verify} messages.
     * @function encodeDelimited
     * @memberof PBDBData
     * @static
     * @param {IPBDBData} message PBDBData message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBDBData.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a PBDBData message from the specified reader or buffer.
     * @function decode
     * @memberof PBDBData
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {PBDBData} PBDBData
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBDBData.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.PBDBData();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 10:
                message.baseInfo = $root.DBBaseInfo.decode(reader, reader.uint32());
                break;
            case 11:
                message.goldInfo = $root.DBGoldInfo.decode(reader, reader.uint32());
                break;
            case 100:
                message.mBaseInfo = $root.DBMBaseInfo.decode(reader, reader.uint32());
                break;
            case 101:
                message.mLipstickInfo = $root.DBMLipstickInfo.decode(reader, reader.uint32());
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a PBDBData message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof PBDBData
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {PBDBData} PBDBData
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBDBData.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a PBDBData message.
     * @function verify
     * @memberof PBDBData
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    PBDBData.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.baseInfo != null && message.hasOwnProperty("baseInfo")) {
            var error = $root.DBBaseInfo.verify(message.baseInfo);
            if (error)
                return "baseInfo." + error;
        }
        if (message.goldInfo != null && message.hasOwnProperty("goldInfo")) {
            var error = $root.DBGoldInfo.verify(message.goldInfo);
            if (error)
                return "goldInfo." + error;
        }
        if (message.mBaseInfo != null && message.hasOwnProperty("mBaseInfo")) {
            var error = $root.DBMBaseInfo.verify(message.mBaseInfo);
            if (error)
                return "mBaseInfo." + error;
        }
        if (message.mLipstickInfo != null && message.hasOwnProperty("mLipstickInfo")) {
            var error = $root.DBMLipstickInfo.verify(message.mLipstickInfo);
            if (error)
                return "mLipstickInfo." + error;
        }
        return null;
    };

    /**
     * Creates a PBDBData message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof PBDBData
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {PBDBData} PBDBData
     */
    PBDBData.fromObject = function fromObject(object) {
        if (object instanceof $root.PBDBData)
            return object;
        var message = new $root.PBDBData();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.baseInfo != null) {
            if (typeof object.baseInfo !== "object")
                throw TypeError(".PBDBData.baseInfo: object expected");
            message.baseInfo = $root.DBBaseInfo.fromObject(object.baseInfo);
        }
        if (object.goldInfo != null) {
            if (typeof object.goldInfo !== "object")
                throw TypeError(".PBDBData.goldInfo: object expected");
            message.goldInfo = $root.DBGoldInfo.fromObject(object.goldInfo);
        }
        if (object.mBaseInfo != null) {
            if (typeof object.mBaseInfo !== "object")
                throw TypeError(".PBDBData.mBaseInfo: object expected");
            message.mBaseInfo = $root.DBMBaseInfo.fromObject(object.mBaseInfo);
        }
        if (object.mLipstickInfo != null) {
            if (typeof object.mLipstickInfo !== "object")
                throw TypeError(".PBDBData.mLipstickInfo: object expected");
            message.mLipstickInfo = $root.DBMLipstickInfo.fromObject(object.mLipstickInfo);
        }
        return message;
    };

    /**
     * Creates a plain object from a PBDBData message. Also converts values to other types if specified.
     * @function toObject
     * @memberof PBDBData
     * @static
     * @param {PBDBData} message PBDBData
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    PBDBData.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
            object.baseInfo = null;
            object.goldInfo = null;
            object.mBaseInfo = null;
            object.mLipstickInfo = null;
        }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.baseInfo != null && message.hasOwnProperty("baseInfo"))
            object.baseInfo = $root.DBBaseInfo.toObject(message.baseInfo, options);
        if (message.goldInfo != null && message.hasOwnProperty("goldInfo"))
            object.goldInfo = $root.DBGoldInfo.toObject(message.goldInfo, options);
        if (message.mBaseInfo != null && message.hasOwnProperty("mBaseInfo"))
            object.mBaseInfo = $root.DBMBaseInfo.toObject(message.mBaseInfo, options);
        if (message.mLipstickInfo != null && message.hasOwnProperty("mLipstickInfo"))
            object.mLipstickInfo = $root.DBMLipstickInfo.toObject(message.mLipstickInfo, options);
        return object;
    };

    /**
     * Converts this PBDBData to JSON.
     * @function toJSON
     * @memberof PBDBData
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    PBDBData.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return PBDBData;
})();

$root.PBDBKey = (function() {

    /**
     * Properties of a PBDBKey.
     * @exports IPBDBKey
     * @interface IPBDBKey
     * @property {IDBBaseInfo|null} [baseInfo] PBDBKey baseInfo
     * @property {IDBGoldInfo|null} [goldInfo] PBDBKey goldInfo
     * @property {IDBMBaseInfo|null} [mBaseInfo] PBDBKey mBaseInfo
     * @property {IDBMLipstickInfo|null} [mLipstickInfo] PBDBKey mLipstickInfo
     */

    /**
     * Constructs a new PBDBKey.
     * @exports PBDBKey
     * @classdesc Represents a PBDBKey.
     * @implements IPBDBKey
     * @constructor
     * @param {IPBDBKey=} [properties] Properties to set
     */
    function PBDBKey(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * PBDBKey baseInfo.
     * @member {IDBBaseInfo|null|undefined} baseInfo
     * @memberof PBDBKey
     * @instance
     */
    PBDBKey.prototype.baseInfo = null;

    /**
     * PBDBKey goldInfo.
     * @member {IDBGoldInfo|null|undefined} goldInfo
     * @memberof PBDBKey
     * @instance
     */
    PBDBKey.prototype.goldInfo = null;

    /**
     * PBDBKey mBaseInfo.
     * @member {IDBMBaseInfo|null|undefined} mBaseInfo
     * @memberof PBDBKey
     * @instance
     */
    PBDBKey.prototype.mBaseInfo = null;

    /**
     * PBDBKey mLipstickInfo.
     * @member {IDBMLipstickInfo|null|undefined} mLipstickInfo
     * @memberof PBDBKey
     * @instance
     */
    PBDBKey.prototype.mLipstickInfo = null;

    // OneOf field names bound to virtual getters and setters
    var $oneOfFields;

    /**
     * PBDBKey DBKeyUnion.
     * @member {"baseInfo"|"goldInfo"|"mBaseInfo"|"mLipstickInfo"|undefined} DBKeyUnion
     * @memberof PBDBKey
     * @instance
     */
    Object.defineProperty(PBDBKey.prototype, "DBKeyUnion", {
        get: $util.oneOfGetter($oneOfFields = ["baseInfo", "goldInfo", "mBaseInfo", "mLipstickInfo"]),
        set: $util.oneOfSetter($oneOfFields)
    });

    /**
     * Creates a new PBDBKey instance using the specified properties.
     * @function create
     * @memberof PBDBKey
     * @static
     * @param {IPBDBKey=} [properties] Properties to set
     * @returns {PBDBKey} PBDBKey instance
     */
    PBDBKey.create = function create(properties) {
        return new PBDBKey(properties);
    };

    /**
     * Encodes the specified PBDBKey message. Does not implicitly {@link PBDBKey.verify|verify} messages.
     * @function encode
     * @memberof PBDBKey
     * @static
     * @param {IPBDBKey} message PBDBKey message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBDBKey.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.baseInfo != null && message.hasOwnProperty("baseInfo"))
            $root.DBBaseInfo.encode(message.baseInfo, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
        if (message.goldInfo != null && message.hasOwnProperty("goldInfo"))
            $root.DBGoldInfo.encode(message.goldInfo, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
        if (message.mBaseInfo != null && message.hasOwnProperty("mBaseInfo"))
            $root.DBMBaseInfo.encode(message.mBaseInfo, writer.uint32(/* id 100, wireType 2 =*/802).fork()).ldelim();
        if (message.mLipstickInfo != null && message.hasOwnProperty("mLipstickInfo"))
            $root.DBMLipstickInfo.encode(message.mLipstickInfo, writer.uint32(/* id 101, wireType 2 =*/810).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified PBDBKey message, length delimited. Does not implicitly {@link PBDBKey.verify|verify} messages.
     * @function encodeDelimited
     * @memberof PBDBKey
     * @static
     * @param {IPBDBKey} message PBDBKey message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBDBKey.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a PBDBKey message from the specified reader or buffer.
     * @function decode
     * @memberof PBDBKey
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {PBDBKey} PBDBKey
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBDBKey.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.PBDBKey();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 10:
                message.baseInfo = $root.DBBaseInfo.decode(reader, reader.uint32());
                break;
            case 11:
                message.goldInfo = $root.DBGoldInfo.decode(reader, reader.uint32());
                break;
            case 100:
                message.mBaseInfo = $root.DBMBaseInfo.decode(reader, reader.uint32());
                break;
            case 101:
                message.mLipstickInfo = $root.DBMLipstickInfo.decode(reader, reader.uint32());
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a PBDBKey message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof PBDBKey
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {PBDBKey} PBDBKey
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBDBKey.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a PBDBKey message.
     * @function verify
     * @memberof PBDBKey
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    PBDBKey.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        var properties = {};
        if (message.baseInfo != null && message.hasOwnProperty("baseInfo")) {
            properties.DBKeyUnion = 1;
            {
                var error = $root.DBBaseInfo.verify(message.baseInfo);
                if (error)
                    return "baseInfo." + error;
            }
        }
        if (message.goldInfo != null && message.hasOwnProperty("goldInfo")) {
            if (properties.DBKeyUnion === 1)
                return "DBKeyUnion: multiple values";
            properties.DBKeyUnion = 1;
            {
                var error = $root.DBGoldInfo.verify(message.goldInfo);
                if (error)
                    return "goldInfo." + error;
            }
        }
        if (message.mBaseInfo != null && message.hasOwnProperty("mBaseInfo")) {
            if (properties.DBKeyUnion === 1)
                return "DBKeyUnion: multiple values";
            properties.DBKeyUnion = 1;
            {
                var error = $root.DBMBaseInfo.verify(message.mBaseInfo);
                if (error)
                    return "mBaseInfo." + error;
            }
        }
        if (message.mLipstickInfo != null && message.hasOwnProperty("mLipstickInfo")) {
            if (properties.DBKeyUnion === 1)
                return "DBKeyUnion: multiple values";
            properties.DBKeyUnion = 1;
            {
                var error = $root.DBMLipstickInfo.verify(message.mLipstickInfo);
                if (error)
                    return "mLipstickInfo." + error;
            }
        }
        return null;
    };

    /**
     * Creates a PBDBKey message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof PBDBKey
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {PBDBKey} PBDBKey
     */
    PBDBKey.fromObject = function fromObject(object) {
        if (object instanceof $root.PBDBKey)
            return object;
        var message = new $root.PBDBKey();
        if (object.baseInfo != null) {
            if (typeof object.baseInfo !== "object")
                throw TypeError(".PBDBKey.baseInfo: object expected");
            message.baseInfo = $root.DBBaseInfo.fromObject(object.baseInfo);
        }
        if (object.goldInfo != null) {
            if (typeof object.goldInfo !== "object")
                throw TypeError(".PBDBKey.goldInfo: object expected");
            message.goldInfo = $root.DBGoldInfo.fromObject(object.goldInfo);
        }
        if (object.mBaseInfo != null) {
            if (typeof object.mBaseInfo !== "object")
                throw TypeError(".PBDBKey.mBaseInfo: object expected");
            message.mBaseInfo = $root.DBMBaseInfo.fromObject(object.mBaseInfo);
        }
        if (object.mLipstickInfo != null) {
            if (typeof object.mLipstickInfo !== "object")
                throw TypeError(".PBDBKey.mLipstickInfo: object expected");
            message.mLipstickInfo = $root.DBMLipstickInfo.fromObject(object.mLipstickInfo);
        }
        return message;
    };

    /**
     * Creates a plain object from a PBDBKey message. Also converts values to other types if specified.
     * @function toObject
     * @memberof PBDBKey
     * @static
     * @param {PBDBKey} message PBDBKey
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    PBDBKey.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (message.baseInfo != null && message.hasOwnProperty("baseInfo")) {
            object.baseInfo = $root.DBBaseInfo.toObject(message.baseInfo, options);
            if (options.oneofs)
                object.DBKeyUnion = "baseInfo";
        }
        if (message.goldInfo != null && message.hasOwnProperty("goldInfo")) {
            object.goldInfo = $root.DBGoldInfo.toObject(message.goldInfo, options);
            if (options.oneofs)
                object.DBKeyUnion = "goldInfo";
        }
        if (message.mBaseInfo != null && message.hasOwnProperty("mBaseInfo")) {
            object.mBaseInfo = $root.DBMBaseInfo.toObject(message.mBaseInfo, options);
            if (options.oneofs)
                object.DBKeyUnion = "mBaseInfo";
        }
        if (message.mLipstickInfo != null && message.hasOwnProperty("mLipstickInfo")) {
            object.mLipstickInfo = $root.DBMLipstickInfo.toObject(message.mLipstickInfo, options);
            if (options.oneofs)
                object.DBKeyUnion = "mLipstickInfo";
        }
        return object;
    };

    /**
     * Converts this PBDBKey to JSON.
     * @function toJSON
     * @memberof PBDBKey
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    PBDBKey.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return PBDBKey;
})();

/**
 * ENKVDBResult enum.
 * @exports ENKVDBResult
 * @enum {string}
 * @property {number} EN_KVDB_Sucess=1 EN_KVDB_Sucess value
 * @property {number} EN_KVDB_Failed=2 EN_KVDB_Failed value
 * @property {number} EN_KVDB_Ver_Diff=3 EN_KVDB_Ver_Diff value
 */
$root.ENKVDBResult = (function() {
    var valuesById = {}, values = Object.create(valuesById);
    values[valuesById[1] = "EN_KVDB_Sucess"] = 1;
    values[valuesById[2] = "EN_KVDB_Failed"] = 2;
    values[valuesById[3] = "EN_KVDB_Ver_Diff"] = 3;
    return values;
})();

$root.PBKVDBData = (function() {

    /**
     * Properties of a PBKVDBData.
     * @exports IPBKVDBData
     * @interface IPBKVDBData
     * @property {number|null} [key] PBKVDBData key
     * @property {Uint8Array|null} [value] PBKVDBData value
     * @property {number|null} [version] PBKVDBData version
     * @property {number|null} [updateTime] PBKVDBData updateTime
     */

    /**
     * Constructs a new PBKVDBData.
     * @exports PBKVDBData
     * @classdesc Represents a PBKVDBData.
     * @implements IPBKVDBData
     * @constructor
     * @param {IPBKVDBData=} [properties] Properties to set
     */
    function PBKVDBData(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * PBKVDBData key.
     * @member {number} key
     * @memberof PBKVDBData
     * @instance
     */
    PBKVDBData.prototype.key = 0;

    /**
     * PBKVDBData value.
     * @member {Uint8Array} value
     * @memberof PBKVDBData
     * @instance
     */
    PBKVDBData.prototype.value = $util.newBuffer([]);

    /**
     * PBKVDBData version.
     * @member {number} version
     * @memberof PBKVDBData
     * @instance
     */
    PBKVDBData.prototype.version = 0;

    /**
     * PBKVDBData updateTime.
     * @member {number} updateTime
     * @memberof PBKVDBData
     * @instance
     */
    PBKVDBData.prototype.updateTime = 0;

    /**
     * Creates a new PBKVDBData instance using the specified properties.
     * @function create
     * @memberof PBKVDBData
     * @static
     * @param {IPBKVDBData=} [properties] Properties to set
     * @returns {PBKVDBData} PBKVDBData instance
     */
    PBKVDBData.create = function create(properties) {
        return new PBKVDBData(properties);
    };

    /**
     * Encodes the specified PBKVDBData message. Does not implicitly {@link PBKVDBData.verify|verify} messages.
     * @function encode
     * @memberof PBKVDBData
     * @static
     * @param {IPBKVDBData} message PBKVDBData message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBKVDBData.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.key != null && message.hasOwnProperty("key"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.key);
        if (message.value != null && message.hasOwnProperty("value"))
            writer.uint32(/* id 2, wireType 2 =*/18).bytes(message.value);
        if (message.version != null && message.hasOwnProperty("version"))
            writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.version);
        if (message.updateTime != null && message.hasOwnProperty("updateTime"))
            writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.updateTime);
        return writer;
    };

    /**
     * Encodes the specified PBKVDBData message, length delimited. Does not implicitly {@link PBKVDBData.verify|verify} messages.
     * @function encodeDelimited
     * @memberof PBKVDBData
     * @static
     * @param {IPBKVDBData} message PBKVDBData message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBKVDBData.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a PBKVDBData message from the specified reader or buffer.
     * @function decode
     * @memberof PBKVDBData
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {PBKVDBData} PBKVDBData
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBKVDBData.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.PBKVDBData();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.key = reader.uint32();
                break;
            case 2:
                message.value = reader.bytes();
                break;
            case 3:
                message.version = reader.uint32();
                break;
            case 4:
                message.updateTime = reader.uint32();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a PBKVDBData message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof PBKVDBData
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {PBKVDBData} PBKVDBData
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBKVDBData.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a PBKVDBData message.
     * @function verify
     * @memberof PBKVDBData
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    PBKVDBData.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.key != null && message.hasOwnProperty("key"))
            if (!$util.isInteger(message.key))
                return "key: integer expected";
        if (message.value != null && message.hasOwnProperty("value"))
            if (!(message.value && typeof message.value.length === "number" || $util.isString(message.value)))
                return "value: buffer expected";
        if (message.version != null && message.hasOwnProperty("version"))
            if (!$util.isInteger(message.version))
                return "version: integer expected";
        if (message.updateTime != null && message.hasOwnProperty("updateTime"))
            if (!$util.isInteger(message.updateTime))
                return "updateTime: integer expected";
        return null;
    };

    /**
     * Creates a PBKVDBData message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof PBKVDBData
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {PBKVDBData} PBKVDBData
     */
    PBKVDBData.fromObject = function fromObject(object) {
        if (object instanceof $root.PBKVDBData)
            return object;
        var message = new $root.PBKVDBData();
        if (object.key != null)
            message.key = object.key >>> 0;
        if (object.value != null)
            if (typeof object.value === "string")
                $util.base64.decode(object.value, message.value = $util.newBuffer($util.base64.length(object.value)), 0);
            else if (object.value.length)
                message.value = object.value;
        if (object.version != null)
            message.version = object.version >>> 0;
        if (object.updateTime != null)
            message.updateTime = object.updateTime >>> 0;
        return message;
    };

    /**
     * Creates a plain object from a PBKVDBData message. Also converts values to other types if specified.
     * @function toObject
     * @memberof PBKVDBData
     * @static
     * @param {PBKVDBData} message PBKVDBData
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    PBKVDBData.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            object.key = 0;
            if (options.bytes === String)
                object.value = "";
            else {
                object.value = [];
                if (options.bytes !== Array)
                    object.value = $util.newBuffer(object.value);
            }
            object.version = 0;
            object.updateTime = 0;
        }
        if (message.key != null && message.hasOwnProperty("key"))
            object.key = message.key;
        if (message.value != null && message.hasOwnProperty("value"))
            object.value = options.bytes === String ? $util.base64.encode(message.value, 0, message.value.length) : options.bytes === Array ? Array.prototype.slice.call(message.value) : message.value;
        if (message.version != null && message.hasOwnProperty("version"))
            object.version = message.version;
        if (message.updateTime != null && message.hasOwnProperty("updateTime"))
            object.updateTime = message.updateTime;
        return object;
    };

    /**
     * Converts this PBKVDBData to JSON.
     * @function toJSON
     * @memberof PBKVDBData
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    PBKVDBData.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return PBKVDBData;
})();

$root.SSReqQuery = (function() {

    /**
     * Properties of a SSReqQuery.
     * @exports ISSReqQuery
     * @interface ISSReqQuery
     * @property {number|Long|null} [uid] SSReqQuery uid
     * @property {Array.<IPBKVDBData>|null} [datas] SSReqQuery datas
     */

    /**
     * Constructs a new SSReqQuery.
     * @exports SSReqQuery
     * @classdesc Represents a SSReqQuery.
     * @implements ISSReqQuery
     * @constructor
     * @param {ISSReqQuery=} [properties] Properties to set
     */
    function SSReqQuery(properties) {
        this.datas = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * SSReqQuery uid.
     * @member {number|Long} uid
     * @memberof SSReqQuery
     * @instance
     */
    SSReqQuery.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * SSReqQuery datas.
     * @member {Array.<IPBKVDBData>} datas
     * @memberof SSReqQuery
     * @instance
     */
    SSReqQuery.prototype.datas = $util.emptyArray;

    /**
     * Creates a new SSReqQuery instance using the specified properties.
     * @function create
     * @memberof SSReqQuery
     * @static
     * @param {ISSReqQuery=} [properties] Properties to set
     * @returns {SSReqQuery} SSReqQuery instance
     */
    SSReqQuery.create = function create(properties) {
        return new SSReqQuery(properties);
    };

    /**
     * Encodes the specified SSReqQuery message. Does not implicitly {@link SSReqQuery.verify|verify} messages.
     * @function encode
     * @memberof SSReqQuery
     * @static
     * @param {ISSReqQuery} message SSReqQuery message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSReqQuery.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.datas != null && message.datas.length)
            for (var i = 0; i < message.datas.length; ++i)
                $root.PBKVDBData.encode(message.datas[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified SSReqQuery message, length delimited. Does not implicitly {@link SSReqQuery.verify|verify} messages.
     * @function encodeDelimited
     * @memberof SSReqQuery
     * @static
     * @param {ISSReqQuery} message SSReqQuery message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSReqQuery.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a SSReqQuery message from the specified reader or buffer.
     * @function decode
     * @memberof SSReqQuery
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {SSReqQuery} SSReqQuery
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSReqQuery.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.SSReqQuery();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                if (!(message.datas && message.datas.length))
                    message.datas = [];
                message.datas.push($root.PBKVDBData.decode(reader, reader.uint32()));
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a SSReqQuery message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof SSReqQuery
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {SSReqQuery} SSReqQuery
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSReqQuery.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a SSReqQuery message.
     * @function verify
     * @memberof SSReqQuery
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    SSReqQuery.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.datas != null && message.hasOwnProperty("datas")) {
            if (!Array.isArray(message.datas))
                return "datas: array expected";
            for (var i = 0; i < message.datas.length; ++i) {
                var error = $root.PBKVDBData.verify(message.datas[i]);
                if (error)
                    return "datas." + error;
            }
        }
        return null;
    };

    /**
     * Creates a SSReqQuery message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof SSReqQuery
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {SSReqQuery} SSReqQuery
     */
    SSReqQuery.fromObject = function fromObject(object) {
        if (object instanceof $root.SSReqQuery)
            return object;
        var message = new $root.SSReqQuery();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.datas) {
            if (!Array.isArray(object.datas))
                throw TypeError(".SSReqQuery.datas: array expected");
            message.datas = [];
            for (var i = 0; i < object.datas.length; ++i) {
                if (typeof object.datas[i] !== "object")
                    throw TypeError(".SSReqQuery.datas: object expected");
                message.datas[i] = $root.PBKVDBData.fromObject(object.datas[i]);
            }
        }
        return message;
    };

    /**
     * Creates a plain object from a SSReqQuery message. Also converts values to other types if specified.
     * @function toObject
     * @memberof SSReqQuery
     * @static
     * @param {SSReqQuery} message SSReqQuery
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    SSReqQuery.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.datas = [];
        if (options.defaults)
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.datas && message.datas.length) {
            object.datas = [];
            for (var j = 0; j < message.datas.length; ++j)
                object.datas[j] = $root.PBKVDBData.toObject(message.datas[j], options);
        }
        return object;
    };

    /**
     * Converts this SSReqQuery to JSON.
     * @function toJSON
     * @memberof SSReqQuery
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    SSReqQuery.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return SSReqQuery;
})();

$root.SSRspQuery = (function() {

    /**
     * Properties of a SSRspQuery.
     * @exports ISSRspQuery
     * @interface ISSRspQuery
     * @property {ENKVDBResult|null} [result] SSRspQuery result
     * @property {number|Long|null} [uid] SSRspQuery uid
     * @property {Array.<IPBKVDBData>|null} [datas] SSRspQuery datas
     */

    /**
     * Constructs a new SSRspQuery.
     * @exports SSRspQuery
     * @classdesc Represents a SSRspQuery.
     * @implements ISSRspQuery
     * @constructor
     * @param {ISSRspQuery=} [properties] Properties to set
     */
    function SSRspQuery(properties) {
        this.datas = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * SSRspQuery result.
     * @member {ENKVDBResult} result
     * @memberof SSRspQuery
     * @instance
     */
    SSRspQuery.prototype.result = 1;

    /**
     * SSRspQuery uid.
     * @member {number|Long} uid
     * @memberof SSRspQuery
     * @instance
     */
    SSRspQuery.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * SSRspQuery datas.
     * @member {Array.<IPBKVDBData>} datas
     * @memberof SSRspQuery
     * @instance
     */
    SSRspQuery.prototype.datas = $util.emptyArray;

    /**
     * Creates a new SSRspQuery instance using the specified properties.
     * @function create
     * @memberof SSRspQuery
     * @static
     * @param {ISSRspQuery=} [properties] Properties to set
     * @returns {SSRspQuery} SSRspQuery instance
     */
    SSRspQuery.create = function create(properties) {
        return new SSRspQuery(properties);
    };

    /**
     * Encodes the specified SSRspQuery message. Does not implicitly {@link SSRspQuery.verify|verify} messages.
     * @function encode
     * @memberof SSRspQuery
     * @static
     * @param {ISSRspQuery} message SSRspQuery message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSRspQuery.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.result != null && message.hasOwnProperty("result"))
            writer.uint32(/* id 1, wireType 0 =*/8).int32(message.result);
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.uid);
        if (message.datas != null && message.datas.length)
            for (var i = 0; i < message.datas.length; ++i)
                $root.PBKVDBData.encode(message.datas[i], writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified SSRspQuery message, length delimited. Does not implicitly {@link SSRspQuery.verify|verify} messages.
     * @function encodeDelimited
     * @memberof SSRspQuery
     * @static
     * @param {ISSRspQuery} message SSRspQuery message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSRspQuery.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a SSRspQuery message from the specified reader or buffer.
     * @function decode
     * @memberof SSRspQuery
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {SSRspQuery} SSRspQuery
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSRspQuery.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.SSRspQuery();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.result = reader.int32();
                break;
            case 2:
                message.uid = reader.uint64();
                break;
            case 3:
                if (!(message.datas && message.datas.length))
                    message.datas = [];
                message.datas.push($root.PBKVDBData.decode(reader, reader.uint32()));
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a SSRspQuery message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof SSRspQuery
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {SSRspQuery} SSRspQuery
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSRspQuery.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a SSRspQuery message.
     * @function verify
     * @memberof SSRspQuery
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    SSRspQuery.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.result != null && message.hasOwnProperty("result"))
            switch (message.result) {
            default:
                return "result: enum value expected";
            case 1:
            case 2:
            case 3:
                break;
            }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.datas != null && message.hasOwnProperty("datas")) {
            if (!Array.isArray(message.datas))
                return "datas: array expected";
            for (var i = 0; i < message.datas.length; ++i) {
                var error = $root.PBKVDBData.verify(message.datas[i]);
                if (error)
                    return "datas." + error;
            }
        }
        return null;
    };

    /**
     * Creates a SSRspQuery message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof SSRspQuery
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {SSRspQuery} SSRspQuery
     */
    SSRspQuery.fromObject = function fromObject(object) {
        if (object instanceof $root.SSRspQuery)
            return object;
        var message = new $root.SSRspQuery();
        switch (object.result) {
        case "EN_KVDB_Sucess":
        case 1:
            message.result = 1;
            break;
        case "EN_KVDB_Failed":
        case 2:
            message.result = 2;
            break;
        case "EN_KVDB_Ver_Diff":
        case 3:
            message.result = 3;
            break;
        }
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.datas) {
            if (!Array.isArray(object.datas))
                throw TypeError(".SSRspQuery.datas: array expected");
            message.datas = [];
            for (var i = 0; i < object.datas.length; ++i) {
                if (typeof object.datas[i] !== "object")
                    throw TypeError(".SSRspQuery.datas: object expected");
                message.datas[i] = $root.PBKVDBData.fromObject(object.datas[i]);
            }
        }
        return message;
    };

    /**
     * Creates a plain object from a SSRspQuery message. Also converts values to other types if specified.
     * @function toObject
     * @memberof SSRspQuery
     * @static
     * @param {SSRspQuery} message SSRspQuery
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    SSRspQuery.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.datas = [];
        if (options.defaults) {
            object.result = options.enums === String ? "EN_KVDB_Sucess" : 1;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
        }
        if (message.result != null && message.hasOwnProperty("result"))
            object.result = options.enums === String ? $root.ENKVDBResult[message.result] : message.result;
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.datas && message.datas.length) {
            object.datas = [];
            for (var j = 0; j < message.datas.length; ++j)
                object.datas[j] = $root.PBKVDBData.toObject(message.datas[j], options);
        }
        return object;
    };

    /**
     * Converts this SSRspQuery to JSON.
     * @function toJSON
     * @memberof SSRspQuery
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    SSRspQuery.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return SSRspQuery;
})();

$root.SSReqSave = (function() {

    /**
     * Properties of a SSReqSave.
     * @exports ISSReqSave
     * @interface ISSReqSave
     * @property {number|Long|null} [uid] SSReqSave uid
     * @property {Array.<IPBKVDBData>|null} [datas] SSReqSave datas
     */

    /**
     * Constructs a new SSReqSave.
     * @exports SSReqSave
     * @classdesc Represents a SSReqSave.
     * @implements ISSReqSave
     * @constructor
     * @param {ISSReqSave=} [properties] Properties to set
     */
    function SSReqSave(properties) {
        this.datas = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * SSReqSave uid.
     * @member {number|Long} uid
     * @memberof SSReqSave
     * @instance
     */
    SSReqSave.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * SSReqSave datas.
     * @member {Array.<IPBKVDBData>} datas
     * @memberof SSReqSave
     * @instance
     */
    SSReqSave.prototype.datas = $util.emptyArray;

    /**
     * Creates a new SSReqSave instance using the specified properties.
     * @function create
     * @memberof SSReqSave
     * @static
     * @param {ISSReqSave=} [properties] Properties to set
     * @returns {SSReqSave} SSReqSave instance
     */
    SSReqSave.create = function create(properties) {
        return new SSReqSave(properties);
    };

    /**
     * Encodes the specified SSReqSave message. Does not implicitly {@link SSReqSave.verify|verify} messages.
     * @function encode
     * @memberof SSReqSave
     * @static
     * @param {ISSReqSave} message SSReqSave message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSReqSave.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.datas != null && message.datas.length)
            for (var i = 0; i < message.datas.length; ++i)
                $root.PBKVDBData.encode(message.datas[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified SSReqSave message, length delimited. Does not implicitly {@link SSReqSave.verify|verify} messages.
     * @function encodeDelimited
     * @memberof SSReqSave
     * @static
     * @param {ISSReqSave} message SSReqSave message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSReqSave.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a SSReqSave message from the specified reader or buffer.
     * @function decode
     * @memberof SSReqSave
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {SSReqSave} SSReqSave
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSReqSave.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.SSReqSave();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                if (!(message.datas && message.datas.length))
                    message.datas = [];
                message.datas.push($root.PBKVDBData.decode(reader, reader.uint32()));
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a SSReqSave message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof SSReqSave
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {SSReqSave} SSReqSave
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSReqSave.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a SSReqSave message.
     * @function verify
     * @memberof SSReqSave
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    SSReqSave.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.datas != null && message.hasOwnProperty("datas")) {
            if (!Array.isArray(message.datas))
                return "datas: array expected";
            for (var i = 0; i < message.datas.length; ++i) {
                var error = $root.PBKVDBData.verify(message.datas[i]);
                if (error)
                    return "datas." + error;
            }
        }
        return null;
    };

    /**
     * Creates a SSReqSave message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof SSReqSave
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {SSReqSave} SSReqSave
     */
    SSReqSave.fromObject = function fromObject(object) {
        if (object instanceof $root.SSReqSave)
            return object;
        var message = new $root.SSReqSave();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.datas) {
            if (!Array.isArray(object.datas))
                throw TypeError(".SSReqSave.datas: array expected");
            message.datas = [];
            for (var i = 0; i < object.datas.length; ++i) {
                if (typeof object.datas[i] !== "object")
                    throw TypeError(".SSReqSave.datas: object expected");
                message.datas[i] = $root.PBKVDBData.fromObject(object.datas[i]);
            }
        }
        return message;
    };

    /**
     * Creates a plain object from a SSReqSave message. Also converts values to other types if specified.
     * @function toObject
     * @memberof SSReqSave
     * @static
     * @param {SSReqSave} message SSReqSave
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    SSReqSave.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.datas = [];
        if (options.defaults)
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.datas && message.datas.length) {
            object.datas = [];
            for (var j = 0; j < message.datas.length; ++j)
                object.datas[j] = $root.PBKVDBData.toObject(message.datas[j], options);
        }
        return object;
    };

    /**
     * Converts this SSReqSave to JSON.
     * @function toJSON
     * @memberof SSReqSave
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    SSReqSave.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return SSReqSave;
})();

$root.SSRspSave = (function() {

    /**
     * Properties of a SSRspSave.
     * @exports ISSRspSave
     * @interface ISSRspSave
     * @property {ENKVDBResult|null} [result] SSRspSave result
     * @property {number|Long|null} [uid] SSRspSave uid
     * @property {Array.<IPBKVDBData>|null} [datas] SSRspSave datas
     */

    /**
     * Constructs a new SSRspSave.
     * @exports SSRspSave
     * @classdesc Represents a SSRspSave.
     * @implements ISSRspSave
     * @constructor
     * @param {ISSRspSave=} [properties] Properties to set
     */
    function SSRspSave(properties) {
        this.datas = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * SSRspSave result.
     * @member {ENKVDBResult} result
     * @memberof SSRspSave
     * @instance
     */
    SSRspSave.prototype.result = 1;

    /**
     * SSRspSave uid.
     * @member {number|Long} uid
     * @memberof SSRspSave
     * @instance
     */
    SSRspSave.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * SSRspSave datas.
     * @member {Array.<IPBKVDBData>} datas
     * @memberof SSRspSave
     * @instance
     */
    SSRspSave.prototype.datas = $util.emptyArray;

    /**
     * Creates a new SSRspSave instance using the specified properties.
     * @function create
     * @memberof SSRspSave
     * @static
     * @param {ISSRspSave=} [properties] Properties to set
     * @returns {SSRspSave} SSRspSave instance
     */
    SSRspSave.create = function create(properties) {
        return new SSRspSave(properties);
    };

    /**
     * Encodes the specified SSRspSave message. Does not implicitly {@link SSRspSave.verify|verify} messages.
     * @function encode
     * @memberof SSRspSave
     * @static
     * @param {ISSRspSave} message SSRspSave message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSRspSave.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.result != null && message.hasOwnProperty("result"))
            writer.uint32(/* id 1, wireType 0 =*/8).int32(message.result);
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.uid);
        if (message.datas != null && message.datas.length)
            for (var i = 0; i < message.datas.length; ++i)
                $root.PBKVDBData.encode(message.datas[i], writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified SSRspSave message, length delimited. Does not implicitly {@link SSRspSave.verify|verify} messages.
     * @function encodeDelimited
     * @memberof SSRspSave
     * @static
     * @param {ISSRspSave} message SSRspSave message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    SSRspSave.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a SSRspSave message from the specified reader or buffer.
     * @function decode
     * @memberof SSRspSave
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {SSRspSave} SSRspSave
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSRspSave.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.SSRspSave();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.result = reader.int32();
                break;
            case 2:
                message.uid = reader.uint64();
                break;
            case 3:
                if (!(message.datas && message.datas.length))
                    message.datas = [];
                message.datas.push($root.PBKVDBData.decode(reader, reader.uint32()));
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a SSRspSave message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof SSRspSave
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {SSRspSave} SSRspSave
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    SSRspSave.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a SSRspSave message.
     * @function verify
     * @memberof SSRspSave
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    SSRspSave.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.result != null && message.hasOwnProperty("result"))
            switch (message.result) {
            default:
                return "result: enum value expected";
            case 1:
            case 2:
            case 3:
                break;
            }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.datas != null && message.hasOwnProperty("datas")) {
            if (!Array.isArray(message.datas))
                return "datas: array expected";
            for (var i = 0; i < message.datas.length; ++i) {
                var error = $root.PBKVDBData.verify(message.datas[i]);
                if (error)
                    return "datas." + error;
            }
        }
        return null;
    };

    /**
     * Creates a SSRspSave message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof SSRspSave
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {SSRspSave} SSRspSave
     */
    SSRspSave.fromObject = function fromObject(object) {
        if (object instanceof $root.SSRspSave)
            return object;
        var message = new $root.SSRspSave();
        switch (object.result) {
        case "EN_KVDB_Sucess":
        case 1:
            message.result = 1;
            break;
        case "EN_KVDB_Failed":
        case 2:
            message.result = 2;
            break;
        case "EN_KVDB_Ver_Diff":
        case 3:
            message.result = 3;
            break;
        }
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.datas) {
            if (!Array.isArray(object.datas))
                throw TypeError(".SSRspSave.datas: array expected");
            message.datas = [];
            for (var i = 0; i < object.datas.length; ++i) {
                if (typeof object.datas[i] !== "object")
                    throw TypeError(".SSRspSave.datas: object expected");
                message.datas[i] = $root.PBKVDBData.fromObject(object.datas[i]);
            }
        }
        return message;
    };

    /**
     * Creates a plain object from a SSRspSave message. Also converts values to other types if specified.
     * @function toObject
     * @memberof SSRspSave
     * @static
     * @param {SSRspSave} message SSRspSave
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    SSRspSave.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.datas = [];
        if (options.defaults) {
            object.result = options.enums === String ? "EN_KVDB_Sucess" : 1;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
        }
        if (message.result != null && message.hasOwnProperty("result"))
            object.result = options.enums === String ? $root.ENKVDBResult[message.result] : message.result;
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.datas && message.datas.length) {
            object.datas = [];
            for (var j = 0; j < message.datas.length; ++j)
                object.datas[j] = $root.PBKVDBData.toObject(message.datas[j], options);
        }
        return object;
    };

    /**
     * Converts this SSRspSave to JSON.
     * @function toJSON
     * @memberof SSRspSave
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    SSRspSave.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return SSRspSave;
})();

/**
 * ENWebResult enum.
 * @exports ENWebResult
 * @enum {string}
 * @property {number} EN_Web_OK=1 EN_Web_OK value
 * @property {number} EN_Web_DB_Error=2 EN_Web_DB_Error value
 * @property {number} EN_Web_Machine_Not_Online=3 EN_Web_Machine_Not_Online value
 * @property {number} EN_Web_Machine_In_Use=4 EN_Web_Machine_In_Use value
 * @property {number} EN_Web_Player_Not_Login=5 EN_Web_Player_Not_Login value
 * @property {number} EN_Web_Player_Gold_Not_Enough=6 EN_Web_Player_Gold_Not_Enough value
 */
$root.ENWebResult = (function() {
    var valuesById = {}, values = Object.create(valuesById);
    values[valuesById[1] = "EN_Web_OK"] = 1;
    values[valuesById[2] = "EN_Web_DB_Error"] = 2;
    values[valuesById[3] = "EN_Web_Machine_Not_Online"] = 3;
    values[valuesById[4] = "EN_Web_Machine_In_Use"] = 4;
    values[valuesById[5] = "EN_Web_Player_Not_Login"] = 5;
    values[valuesById[6] = "EN_Web_Player_Gold_Not_Enough"] = 6;
    return values;
})();

$root.WebReqPlayerLogin = (function() {

    /**
     * Properties of a WebReqPlayerLogin.
     * @exports IWebReqPlayerLogin
     * @interface IWebReqPlayerLogin
     * @property {number|Long|null} [uid] WebReqPlayerLogin uid
     * @property {number|Long|null} [machineId] WebReqPlayerLogin machineId
     */

    /**
     * Constructs a new WebReqPlayerLogin.
     * @exports WebReqPlayerLogin
     * @classdesc Represents a WebReqPlayerLogin.
     * @implements IWebReqPlayerLogin
     * @constructor
     * @param {IWebReqPlayerLogin=} [properties] Properties to set
     */
    function WebReqPlayerLogin(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebReqPlayerLogin uid.
     * @member {number|Long} uid
     * @memberof WebReqPlayerLogin
     * @instance
     */
    WebReqPlayerLogin.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * WebReqPlayerLogin machineId.
     * @member {number|Long} machineId
     * @memberof WebReqPlayerLogin
     * @instance
     */
    WebReqPlayerLogin.prototype.machineId = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * Creates a new WebReqPlayerLogin instance using the specified properties.
     * @function create
     * @memberof WebReqPlayerLogin
     * @static
     * @param {IWebReqPlayerLogin=} [properties] Properties to set
     * @returns {WebReqPlayerLogin} WebReqPlayerLogin instance
     */
    WebReqPlayerLogin.create = function create(properties) {
        return new WebReqPlayerLogin(properties);
    };

    /**
     * Encodes the specified WebReqPlayerLogin message. Does not implicitly {@link WebReqPlayerLogin.verify|verify} messages.
     * @function encode
     * @memberof WebReqPlayerLogin
     * @static
     * @param {IWebReqPlayerLogin} message WebReqPlayerLogin message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebReqPlayerLogin.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.machineId);
        return writer;
    };

    /**
     * Encodes the specified WebReqPlayerLogin message, length delimited. Does not implicitly {@link WebReqPlayerLogin.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebReqPlayerLogin
     * @static
     * @param {IWebReqPlayerLogin} message WebReqPlayerLogin message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebReqPlayerLogin.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebReqPlayerLogin message from the specified reader or buffer.
     * @function decode
     * @memberof WebReqPlayerLogin
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebReqPlayerLogin} WebReqPlayerLogin
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebReqPlayerLogin.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebReqPlayerLogin();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                message.machineId = reader.uint64();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebReqPlayerLogin message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebReqPlayerLogin
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebReqPlayerLogin} WebReqPlayerLogin
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebReqPlayerLogin.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebReqPlayerLogin message.
     * @function verify
     * @memberof WebReqPlayerLogin
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebReqPlayerLogin.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            if (!$util.isInteger(message.machineId) && !(message.machineId && $util.isInteger(message.machineId.low) && $util.isInteger(message.machineId.high)))
                return "machineId: integer|Long expected";
        return null;
    };

    /**
     * Creates a WebReqPlayerLogin message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebReqPlayerLogin
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebReqPlayerLogin} WebReqPlayerLogin
     */
    WebReqPlayerLogin.fromObject = function fromObject(object) {
        if (object instanceof $root.WebReqPlayerLogin)
            return object;
        var message = new $root.WebReqPlayerLogin();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.machineId != null)
            if ($util.Long)
                (message.machineId = $util.Long.fromValue(object.machineId)).unsigned = true;
            else if (typeof object.machineId === "string")
                message.machineId = parseInt(object.machineId, 10);
            else if (typeof object.machineId === "number")
                message.machineId = object.machineId;
            else if (typeof object.machineId === "object")
                message.machineId = new $util.LongBits(object.machineId.low >>> 0, object.machineId.high >>> 0).toNumber(true);
        return message;
    };

    /**
     * Creates a plain object from a WebReqPlayerLogin message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebReqPlayerLogin
     * @static
     * @param {WebReqPlayerLogin} message WebReqPlayerLogin
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebReqPlayerLogin.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.machineId = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.machineId = options.longs === String ? "0" : 0;
        }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            if (typeof message.machineId === "number")
                object.machineId = options.longs === String ? String(message.machineId) : message.machineId;
            else
                object.machineId = options.longs === String ? $util.Long.prototype.toString.call(message.machineId) : options.longs === Number ? new $util.LongBits(message.machineId.low >>> 0, message.machineId.high >>> 0).toNumber(true) : message.machineId;
        return object;
    };

    /**
     * Converts this WebReqPlayerLogin to JSON.
     * @function toJSON
     * @memberof WebReqPlayerLogin
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebReqPlayerLogin.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebReqPlayerLogin;
})();

$root.WebRspPlayerLogin = (function() {

    /**
     * Properties of a WebRspPlayerLogin.
     * @exports IWebRspPlayerLogin
     * @interface IWebRspPlayerLogin
     * @property {ENWebResult|null} [result] WebRspPlayerLogin result
     * @property {IPBDBData|null} [player] WebRspPlayerLogin player
     */

    /**
     * Constructs a new WebRspPlayerLogin.
     * @exports WebRspPlayerLogin
     * @classdesc Represents a WebRspPlayerLogin.
     * @implements IWebRspPlayerLogin
     * @constructor
     * @param {IWebRspPlayerLogin=} [properties] Properties to set
     */
    function WebRspPlayerLogin(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebRspPlayerLogin result.
     * @member {ENWebResult} result
     * @memberof WebRspPlayerLogin
     * @instance
     */
    WebRspPlayerLogin.prototype.result = 1;

    /**
     * WebRspPlayerLogin player.
     * @member {IPBDBData|null|undefined} player
     * @memberof WebRspPlayerLogin
     * @instance
     */
    WebRspPlayerLogin.prototype.player = null;

    /**
     * Creates a new WebRspPlayerLogin instance using the specified properties.
     * @function create
     * @memberof WebRspPlayerLogin
     * @static
     * @param {IWebRspPlayerLogin=} [properties] Properties to set
     * @returns {WebRspPlayerLogin} WebRspPlayerLogin instance
     */
    WebRspPlayerLogin.create = function create(properties) {
        return new WebRspPlayerLogin(properties);
    };

    /**
     * Encodes the specified WebRspPlayerLogin message. Does not implicitly {@link WebRspPlayerLogin.verify|verify} messages.
     * @function encode
     * @memberof WebRspPlayerLogin
     * @static
     * @param {IWebRspPlayerLogin} message WebRspPlayerLogin message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebRspPlayerLogin.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.result != null && message.hasOwnProperty("result"))
            writer.uint32(/* id 1, wireType 0 =*/8).int32(message.result);
        if (message.player != null && message.hasOwnProperty("player"))
            $root.PBDBData.encode(message.player, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified WebRspPlayerLogin message, length delimited. Does not implicitly {@link WebRspPlayerLogin.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebRspPlayerLogin
     * @static
     * @param {IWebRspPlayerLogin} message WebRspPlayerLogin message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebRspPlayerLogin.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebRspPlayerLogin message from the specified reader or buffer.
     * @function decode
     * @memberof WebRspPlayerLogin
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebRspPlayerLogin} WebRspPlayerLogin
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebRspPlayerLogin.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebRspPlayerLogin();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.result = reader.int32();
                break;
            case 2:
                message.player = $root.PBDBData.decode(reader, reader.uint32());
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebRspPlayerLogin message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebRspPlayerLogin
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebRspPlayerLogin} WebRspPlayerLogin
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebRspPlayerLogin.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebRspPlayerLogin message.
     * @function verify
     * @memberof WebRspPlayerLogin
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebRspPlayerLogin.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.result != null && message.hasOwnProperty("result"))
            switch (message.result) {
            default:
                return "result: enum value expected";
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                break;
            }
        if (message.player != null && message.hasOwnProperty("player")) {
            var error = $root.PBDBData.verify(message.player);
            if (error)
                return "player." + error;
        }
        return null;
    };

    /**
     * Creates a WebRspPlayerLogin message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebRspPlayerLogin
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebRspPlayerLogin} WebRspPlayerLogin
     */
    WebRspPlayerLogin.fromObject = function fromObject(object) {
        if (object instanceof $root.WebRspPlayerLogin)
            return object;
        var message = new $root.WebRspPlayerLogin();
        switch (object.result) {
        case "EN_Web_OK":
        case 1:
            message.result = 1;
            break;
        case "EN_Web_DB_Error":
        case 2:
            message.result = 2;
            break;
        case "EN_Web_Machine_Not_Online":
        case 3:
            message.result = 3;
            break;
        case "EN_Web_Machine_In_Use":
        case 4:
            message.result = 4;
            break;
        case "EN_Web_Player_Not_Login":
        case 5:
            message.result = 5;
            break;
        case "EN_Web_Player_Gold_Not_Enough":
        case 6:
            message.result = 6;
            break;
        }
        if (object.player != null) {
            if (typeof object.player !== "object")
                throw TypeError(".WebRspPlayerLogin.player: object expected");
            message.player = $root.PBDBData.fromObject(object.player);
        }
        return message;
    };

    /**
     * Creates a plain object from a WebRspPlayerLogin message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebRspPlayerLogin
     * @static
     * @param {WebRspPlayerLogin} message WebRspPlayerLogin
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebRspPlayerLogin.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            object.result = options.enums === String ? "EN_Web_OK" : 1;
            object.player = null;
        }
        if (message.result != null && message.hasOwnProperty("result"))
            object.result = options.enums === String ? $root.ENWebResult[message.result] : message.result;
        if (message.player != null && message.hasOwnProperty("player"))
            object.player = $root.PBDBData.toObject(message.player, options);
        return object;
    };

    /**
     * Converts this WebRspPlayerLogin to JSON.
     * @function toJSON
     * @memberof WebRspPlayerLogin
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebRspPlayerLogin.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebRspPlayerLogin;
})();

$root.WebReqPlayerBuyIn = (function() {

    /**
     * Properties of a WebReqPlayerBuyIn.
     * @exports IWebReqPlayerBuyIn
     * @interface IWebReqPlayerBuyIn
     * @property {number|Long|null} [uid] WebReqPlayerBuyIn uid
     * @property {number|Long|null} [machineId] WebReqPlayerBuyIn machineId
     * @property {number|Long|null} [buyIn] WebReqPlayerBuyIn buyIn
     */

    /**
     * Constructs a new WebReqPlayerBuyIn.
     * @exports WebReqPlayerBuyIn
     * @classdesc Represents a WebReqPlayerBuyIn.
     * @implements IWebReqPlayerBuyIn
     * @constructor
     * @param {IWebReqPlayerBuyIn=} [properties] Properties to set
     */
    function WebReqPlayerBuyIn(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebReqPlayerBuyIn uid.
     * @member {number|Long} uid
     * @memberof WebReqPlayerBuyIn
     * @instance
     */
    WebReqPlayerBuyIn.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * WebReqPlayerBuyIn machineId.
     * @member {number|Long} machineId
     * @memberof WebReqPlayerBuyIn
     * @instance
     */
    WebReqPlayerBuyIn.prototype.machineId = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * WebReqPlayerBuyIn buyIn.
     * @member {number|Long} buyIn
     * @memberof WebReqPlayerBuyIn
     * @instance
     */
    WebReqPlayerBuyIn.prototype.buyIn = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * Creates a new WebReqPlayerBuyIn instance using the specified properties.
     * @function create
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {IWebReqPlayerBuyIn=} [properties] Properties to set
     * @returns {WebReqPlayerBuyIn} WebReqPlayerBuyIn instance
     */
    WebReqPlayerBuyIn.create = function create(properties) {
        return new WebReqPlayerBuyIn(properties);
    };

    /**
     * Encodes the specified WebReqPlayerBuyIn message. Does not implicitly {@link WebReqPlayerBuyIn.verify|verify} messages.
     * @function encode
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {IWebReqPlayerBuyIn} message WebReqPlayerBuyIn message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebReqPlayerBuyIn.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.machineId);
        if (message.buyIn != null && message.hasOwnProperty("buyIn"))
            writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.buyIn);
        return writer;
    };

    /**
     * Encodes the specified WebReqPlayerBuyIn message, length delimited. Does not implicitly {@link WebReqPlayerBuyIn.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {IWebReqPlayerBuyIn} message WebReqPlayerBuyIn message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebReqPlayerBuyIn.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebReqPlayerBuyIn message from the specified reader or buffer.
     * @function decode
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebReqPlayerBuyIn} WebReqPlayerBuyIn
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebReqPlayerBuyIn.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebReqPlayerBuyIn();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                message.machineId = reader.uint64();
                break;
            case 3:
                message.buyIn = reader.uint64();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebReqPlayerBuyIn message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebReqPlayerBuyIn} WebReqPlayerBuyIn
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebReqPlayerBuyIn.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebReqPlayerBuyIn message.
     * @function verify
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebReqPlayerBuyIn.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            if (!$util.isInteger(message.machineId) && !(message.machineId && $util.isInteger(message.machineId.low) && $util.isInteger(message.machineId.high)))
                return "machineId: integer|Long expected";
        if (message.buyIn != null && message.hasOwnProperty("buyIn"))
            if (!$util.isInteger(message.buyIn) && !(message.buyIn && $util.isInteger(message.buyIn.low) && $util.isInteger(message.buyIn.high)))
                return "buyIn: integer|Long expected";
        return null;
    };

    /**
     * Creates a WebReqPlayerBuyIn message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebReqPlayerBuyIn} WebReqPlayerBuyIn
     */
    WebReqPlayerBuyIn.fromObject = function fromObject(object) {
        if (object instanceof $root.WebReqPlayerBuyIn)
            return object;
        var message = new $root.WebReqPlayerBuyIn();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.machineId != null)
            if ($util.Long)
                (message.machineId = $util.Long.fromValue(object.machineId)).unsigned = true;
            else if (typeof object.machineId === "string")
                message.machineId = parseInt(object.machineId, 10);
            else if (typeof object.machineId === "number")
                message.machineId = object.machineId;
            else if (typeof object.machineId === "object")
                message.machineId = new $util.LongBits(object.machineId.low >>> 0, object.machineId.high >>> 0).toNumber(true);
        if (object.buyIn != null)
            if ($util.Long)
                (message.buyIn = $util.Long.fromValue(object.buyIn)).unsigned = true;
            else if (typeof object.buyIn === "string")
                message.buyIn = parseInt(object.buyIn, 10);
            else if (typeof object.buyIn === "number")
                message.buyIn = object.buyIn;
            else if (typeof object.buyIn === "object")
                message.buyIn = new $util.LongBits(object.buyIn.low >>> 0, object.buyIn.high >>> 0).toNumber(true);
        return message;
    };

    /**
     * Creates a plain object from a WebReqPlayerBuyIn message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebReqPlayerBuyIn
     * @static
     * @param {WebReqPlayerBuyIn} message WebReqPlayerBuyIn
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebReqPlayerBuyIn.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.machineId = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.machineId = options.longs === String ? "0" : 0;
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.buyIn = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.buyIn = options.longs === String ? "0" : 0;
        }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.machineId != null && message.hasOwnProperty("machineId"))
            if (typeof message.machineId === "number")
                object.machineId = options.longs === String ? String(message.machineId) : message.machineId;
            else
                object.machineId = options.longs === String ? $util.Long.prototype.toString.call(message.machineId) : options.longs === Number ? new $util.LongBits(message.machineId.low >>> 0, message.machineId.high >>> 0).toNumber(true) : message.machineId;
        if (message.buyIn != null && message.hasOwnProperty("buyIn"))
            if (typeof message.buyIn === "number")
                object.buyIn = options.longs === String ? String(message.buyIn) : message.buyIn;
            else
                object.buyIn = options.longs === String ? $util.Long.prototype.toString.call(message.buyIn) : options.longs === Number ? new $util.LongBits(message.buyIn.low >>> 0, message.buyIn.high >>> 0).toNumber(true) : message.buyIn;
        return object;
    };

    /**
     * Converts this WebReqPlayerBuyIn to JSON.
     * @function toJSON
     * @memberof WebReqPlayerBuyIn
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebReqPlayerBuyIn.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebReqPlayerBuyIn;
})();

$root.WebRspPlayerBuyIn = (function() {

    /**
     * Properties of a WebRspPlayerBuyIn.
     * @exports IWebRspPlayerBuyIn
     * @interface IWebRspPlayerBuyIn
     * @property {ENWebResult|null} [result] WebRspPlayerBuyIn result
     */

    /**
     * Constructs a new WebRspPlayerBuyIn.
     * @exports WebRspPlayerBuyIn
     * @classdesc Represents a WebRspPlayerBuyIn.
     * @implements IWebRspPlayerBuyIn
     * @constructor
     * @param {IWebRspPlayerBuyIn=} [properties] Properties to set
     */
    function WebRspPlayerBuyIn(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebRspPlayerBuyIn result.
     * @member {ENWebResult} result
     * @memberof WebRspPlayerBuyIn
     * @instance
     */
    WebRspPlayerBuyIn.prototype.result = 1;

    /**
     * Creates a new WebRspPlayerBuyIn instance using the specified properties.
     * @function create
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {IWebRspPlayerBuyIn=} [properties] Properties to set
     * @returns {WebRspPlayerBuyIn} WebRspPlayerBuyIn instance
     */
    WebRspPlayerBuyIn.create = function create(properties) {
        return new WebRspPlayerBuyIn(properties);
    };

    /**
     * Encodes the specified WebRspPlayerBuyIn message. Does not implicitly {@link WebRspPlayerBuyIn.verify|verify} messages.
     * @function encode
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {IWebRspPlayerBuyIn} message WebRspPlayerBuyIn message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebRspPlayerBuyIn.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.result != null && message.hasOwnProperty("result"))
            writer.uint32(/* id 1, wireType 0 =*/8).int32(message.result);
        return writer;
    };

    /**
     * Encodes the specified WebRspPlayerBuyIn message, length delimited. Does not implicitly {@link WebRspPlayerBuyIn.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {IWebRspPlayerBuyIn} message WebRspPlayerBuyIn message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebRspPlayerBuyIn.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebRspPlayerBuyIn message from the specified reader or buffer.
     * @function decode
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebRspPlayerBuyIn} WebRspPlayerBuyIn
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebRspPlayerBuyIn.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebRspPlayerBuyIn();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.result = reader.int32();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebRspPlayerBuyIn message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebRspPlayerBuyIn} WebRspPlayerBuyIn
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebRspPlayerBuyIn.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebRspPlayerBuyIn message.
     * @function verify
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebRspPlayerBuyIn.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.result != null && message.hasOwnProperty("result"))
            switch (message.result) {
            default:
                return "result: enum value expected";
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                break;
            }
        return null;
    };

    /**
     * Creates a WebRspPlayerBuyIn message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebRspPlayerBuyIn} WebRspPlayerBuyIn
     */
    WebRspPlayerBuyIn.fromObject = function fromObject(object) {
        if (object instanceof $root.WebRspPlayerBuyIn)
            return object;
        var message = new $root.WebRspPlayerBuyIn();
        switch (object.result) {
        case "EN_Web_OK":
        case 1:
            message.result = 1;
            break;
        case "EN_Web_DB_Error":
        case 2:
            message.result = 2;
            break;
        case "EN_Web_Machine_Not_Online":
        case 3:
            message.result = 3;
            break;
        case "EN_Web_Machine_In_Use":
        case 4:
            message.result = 4;
            break;
        case "EN_Web_Player_Not_Login":
        case 5:
            message.result = 5;
            break;
        case "EN_Web_Player_Gold_Not_Enough":
        case 6:
            message.result = 6;
            break;
        }
        return message;
    };

    /**
     * Creates a plain object from a WebRspPlayerBuyIn message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebRspPlayerBuyIn
     * @static
     * @param {WebRspPlayerBuyIn} message WebRspPlayerBuyIn
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebRspPlayerBuyIn.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults)
            object.result = options.enums === String ? "EN_Web_OK" : 1;
        if (message.result != null && message.hasOwnProperty("result"))
            object.result = options.enums === String ? $root.ENWebResult[message.result] : message.result;
        return object;
    };

    /**
     * Converts this WebRspPlayerBuyIn to JSON.
     * @function toJSON
     * @memberof WebRspPlayerBuyIn
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebRspPlayerBuyIn.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebRspPlayerBuyIn;
})();

$root.WebNotifyPlayerInfo = (function() {

    /**
     * Properties of a WebNotifyPlayerInfo.
     * @exports IWebNotifyPlayerInfo
     * @interface IWebNotifyPlayerInfo
     * @property {number|Long|null} [uid] WebNotifyPlayerInfo uid
     * @property {string|null} [nick] WebNotifyPlayerInfo nick
     * @property {string|null} [picUrl] WebNotifyPlayerInfo picUrl
     */

    /**
     * Constructs a new WebNotifyPlayerInfo.
     * @exports WebNotifyPlayerInfo
     * @classdesc Represents a WebNotifyPlayerInfo.
     * @implements IWebNotifyPlayerInfo
     * @constructor
     * @param {IWebNotifyPlayerInfo=} [properties] Properties to set
     */
    function WebNotifyPlayerInfo(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebNotifyPlayerInfo uid.
     * @member {number|Long} uid
     * @memberof WebNotifyPlayerInfo
     * @instance
     */
    WebNotifyPlayerInfo.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * WebNotifyPlayerInfo nick.
     * @member {string} nick
     * @memberof WebNotifyPlayerInfo
     * @instance
     */
    WebNotifyPlayerInfo.prototype.nick = "";

    /**
     * WebNotifyPlayerInfo picUrl.
     * @member {string} picUrl
     * @memberof WebNotifyPlayerInfo
     * @instance
     */
    WebNotifyPlayerInfo.prototype.picUrl = "";

    /**
     * Creates a new WebNotifyPlayerInfo instance using the specified properties.
     * @function create
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {IWebNotifyPlayerInfo=} [properties] Properties to set
     * @returns {WebNotifyPlayerInfo} WebNotifyPlayerInfo instance
     */
    WebNotifyPlayerInfo.create = function create(properties) {
        return new WebNotifyPlayerInfo(properties);
    };

    /**
     * Encodes the specified WebNotifyPlayerInfo message. Does not implicitly {@link WebNotifyPlayerInfo.verify|verify} messages.
     * @function encode
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {IWebNotifyPlayerInfo} message WebNotifyPlayerInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebNotifyPlayerInfo.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.nick != null && message.hasOwnProperty("nick"))
            writer.uint32(/* id 2, wireType 2 =*/18).string(message.nick);
        if (message.picUrl != null && message.hasOwnProperty("picUrl"))
            writer.uint32(/* id 3, wireType 2 =*/26).string(message.picUrl);
        return writer;
    };

    /**
     * Encodes the specified WebNotifyPlayerInfo message, length delimited. Does not implicitly {@link WebNotifyPlayerInfo.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {IWebNotifyPlayerInfo} message WebNotifyPlayerInfo message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebNotifyPlayerInfo.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebNotifyPlayerInfo message from the specified reader or buffer.
     * @function decode
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebNotifyPlayerInfo} WebNotifyPlayerInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebNotifyPlayerInfo.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebNotifyPlayerInfo();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                message.nick = reader.string();
                break;
            case 3:
                message.picUrl = reader.string();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebNotifyPlayerInfo message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebNotifyPlayerInfo} WebNotifyPlayerInfo
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebNotifyPlayerInfo.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebNotifyPlayerInfo message.
     * @function verify
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebNotifyPlayerInfo.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.nick != null && message.hasOwnProperty("nick"))
            if (!$util.isString(message.nick))
                return "nick: string expected";
        if (message.picUrl != null && message.hasOwnProperty("picUrl"))
            if (!$util.isString(message.picUrl))
                return "picUrl: string expected";
        return null;
    };

    /**
     * Creates a WebNotifyPlayerInfo message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebNotifyPlayerInfo} WebNotifyPlayerInfo
     */
    WebNotifyPlayerInfo.fromObject = function fromObject(object) {
        if (object instanceof $root.WebNotifyPlayerInfo)
            return object;
        var message = new $root.WebNotifyPlayerInfo();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.nick != null)
            message.nick = String(object.nick);
        if (object.picUrl != null)
            message.picUrl = String(object.picUrl);
        return message;
    };

    /**
     * Creates a plain object from a WebNotifyPlayerInfo message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebNotifyPlayerInfo
     * @static
     * @param {WebNotifyPlayerInfo} message WebNotifyPlayerInfo
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebNotifyPlayerInfo.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
            object.nick = "";
            object.picUrl = "";
        }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.nick != null && message.hasOwnProperty("nick"))
            object.nick = message.nick;
        if (message.picUrl != null && message.hasOwnProperty("picUrl"))
            object.picUrl = message.picUrl;
        return object;
    };

    /**
     * Converts this WebNotifyPlayerInfo to JSON.
     * @function toJSON
     * @memberof WebNotifyPlayerInfo
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebNotifyPlayerInfo.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebNotifyPlayerInfo;
})();

$root.WebReqPlayerCharge = (function() {

    /**
     * Properties of a WebReqPlayerCharge.
     * @exports IWebReqPlayerCharge
     * @interface IWebReqPlayerCharge
     * @property {number|Long|null} [uid] WebReqPlayerCharge uid
     * @property {Array.<IPBAward>|null} [awards] WebReqPlayerCharge awards
     */

    /**
     * Constructs a new WebReqPlayerCharge.
     * @exports WebReqPlayerCharge
     * @classdesc Represents a WebReqPlayerCharge.
     * @implements IWebReqPlayerCharge
     * @constructor
     * @param {IWebReqPlayerCharge=} [properties] Properties to set
     */
    function WebReqPlayerCharge(properties) {
        this.awards = [];
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebReqPlayerCharge uid.
     * @member {number|Long} uid
     * @memberof WebReqPlayerCharge
     * @instance
     */
    WebReqPlayerCharge.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * WebReqPlayerCharge awards.
     * @member {Array.<IPBAward>} awards
     * @memberof WebReqPlayerCharge
     * @instance
     */
    WebReqPlayerCharge.prototype.awards = $util.emptyArray;

    /**
     * Creates a new WebReqPlayerCharge instance using the specified properties.
     * @function create
     * @memberof WebReqPlayerCharge
     * @static
     * @param {IWebReqPlayerCharge=} [properties] Properties to set
     * @returns {WebReqPlayerCharge} WebReqPlayerCharge instance
     */
    WebReqPlayerCharge.create = function create(properties) {
        return new WebReqPlayerCharge(properties);
    };

    /**
     * Encodes the specified WebReqPlayerCharge message. Does not implicitly {@link WebReqPlayerCharge.verify|verify} messages.
     * @function encode
     * @memberof WebReqPlayerCharge
     * @static
     * @param {IWebReqPlayerCharge} message WebReqPlayerCharge message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebReqPlayerCharge.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.awards != null && message.awards.length)
            for (var i = 0; i < message.awards.length; ++i)
                $root.PBAward.encode(message.awards[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified WebReqPlayerCharge message, length delimited. Does not implicitly {@link WebReqPlayerCharge.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebReqPlayerCharge
     * @static
     * @param {IWebReqPlayerCharge} message WebReqPlayerCharge message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebReqPlayerCharge.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebReqPlayerCharge message from the specified reader or buffer.
     * @function decode
     * @memberof WebReqPlayerCharge
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebReqPlayerCharge} WebReqPlayerCharge
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebReqPlayerCharge.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebReqPlayerCharge();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                if (!(message.awards && message.awards.length))
                    message.awards = [];
                message.awards.push($root.PBAward.decode(reader, reader.uint32()));
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebReqPlayerCharge message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebReqPlayerCharge
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebReqPlayerCharge} WebReqPlayerCharge
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebReqPlayerCharge.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebReqPlayerCharge message.
     * @function verify
     * @memberof WebReqPlayerCharge
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebReqPlayerCharge.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.awards != null && message.hasOwnProperty("awards")) {
            if (!Array.isArray(message.awards))
                return "awards: array expected";
            for (var i = 0; i < message.awards.length; ++i) {
                var error = $root.PBAward.verify(message.awards[i]);
                if (error)
                    return "awards." + error;
            }
        }
        return null;
    };

    /**
     * Creates a WebReqPlayerCharge message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebReqPlayerCharge
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebReqPlayerCharge} WebReqPlayerCharge
     */
    WebReqPlayerCharge.fromObject = function fromObject(object) {
        if (object instanceof $root.WebReqPlayerCharge)
            return object;
        var message = new $root.WebReqPlayerCharge();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        if (object.awards) {
            if (!Array.isArray(object.awards))
                throw TypeError(".WebReqPlayerCharge.awards: array expected");
            message.awards = [];
            for (var i = 0; i < object.awards.length; ++i) {
                if (typeof object.awards[i] !== "object")
                    throw TypeError(".WebReqPlayerCharge.awards: object expected");
                message.awards[i] = $root.PBAward.fromObject(object.awards[i]);
            }
        }
        return message;
    };

    /**
     * Creates a plain object from a WebReqPlayerCharge message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebReqPlayerCharge
     * @static
     * @param {WebReqPlayerCharge} message WebReqPlayerCharge
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebReqPlayerCharge.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.arrays || options.defaults)
            object.awards = [];
        if (options.defaults)
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.awards && message.awards.length) {
            object.awards = [];
            for (var j = 0; j < message.awards.length; ++j)
                object.awards[j] = $root.PBAward.toObject(message.awards[j], options);
        }
        return object;
    };

    /**
     * Converts this WebReqPlayerCharge to JSON.
     * @function toJSON
     * @memberof WebReqPlayerCharge
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebReqPlayerCharge.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebReqPlayerCharge;
})();

$root.WebRspPlayerCharge = (function() {

    /**
     * Properties of a WebRspPlayerCharge.
     * @exports IWebRspPlayerCharge
     * @interface IWebRspPlayerCharge
     * @property {ENWebResult|null} [result] WebRspPlayerCharge result
     */

    /**
     * Constructs a new WebRspPlayerCharge.
     * @exports WebRspPlayerCharge
     * @classdesc Represents a WebRspPlayerCharge.
     * @implements IWebRspPlayerCharge
     * @constructor
     * @param {IWebRspPlayerCharge=} [properties] Properties to set
     */
    function WebRspPlayerCharge(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * WebRspPlayerCharge result.
     * @member {ENWebResult} result
     * @memberof WebRspPlayerCharge
     * @instance
     */
    WebRspPlayerCharge.prototype.result = 1;

    /**
     * Creates a new WebRspPlayerCharge instance using the specified properties.
     * @function create
     * @memberof WebRspPlayerCharge
     * @static
     * @param {IWebRspPlayerCharge=} [properties] Properties to set
     * @returns {WebRspPlayerCharge} WebRspPlayerCharge instance
     */
    WebRspPlayerCharge.create = function create(properties) {
        return new WebRspPlayerCharge(properties);
    };

    /**
     * Encodes the specified WebRspPlayerCharge message. Does not implicitly {@link WebRspPlayerCharge.verify|verify} messages.
     * @function encode
     * @memberof WebRspPlayerCharge
     * @static
     * @param {IWebRspPlayerCharge} message WebRspPlayerCharge message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebRspPlayerCharge.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.result != null && message.hasOwnProperty("result"))
            writer.uint32(/* id 1, wireType 0 =*/8).int32(message.result);
        return writer;
    };

    /**
     * Encodes the specified WebRspPlayerCharge message, length delimited. Does not implicitly {@link WebRspPlayerCharge.verify|verify} messages.
     * @function encodeDelimited
     * @memberof WebRspPlayerCharge
     * @static
     * @param {IWebRspPlayerCharge} message WebRspPlayerCharge message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    WebRspPlayerCharge.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a WebRspPlayerCharge message from the specified reader or buffer.
     * @function decode
     * @memberof WebRspPlayerCharge
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {WebRspPlayerCharge} WebRspPlayerCharge
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebRspPlayerCharge.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.WebRspPlayerCharge();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.result = reader.int32();
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a WebRspPlayerCharge message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof WebRspPlayerCharge
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {WebRspPlayerCharge} WebRspPlayerCharge
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    WebRspPlayerCharge.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a WebRspPlayerCharge message.
     * @function verify
     * @memberof WebRspPlayerCharge
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    WebRspPlayerCharge.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        if (message.result != null && message.hasOwnProperty("result"))
            switch (message.result) {
            default:
                return "result: enum value expected";
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                break;
            }
        return null;
    };

    /**
     * Creates a WebRspPlayerCharge message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof WebRspPlayerCharge
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {WebRspPlayerCharge} WebRspPlayerCharge
     */
    WebRspPlayerCharge.fromObject = function fromObject(object) {
        if (object instanceof $root.WebRspPlayerCharge)
            return object;
        var message = new $root.WebRspPlayerCharge();
        switch (object.result) {
        case "EN_Web_OK":
        case 1:
            message.result = 1;
            break;
        case "EN_Web_DB_Error":
        case 2:
            message.result = 2;
            break;
        case "EN_Web_Machine_Not_Online":
        case 3:
            message.result = 3;
            break;
        case "EN_Web_Machine_In_Use":
        case 4:
            message.result = 4;
            break;
        case "EN_Web_Player_Not_Login":
        case 5:
            message.result = 5;
            break;
        case "EN_Web_Player_Gold_Not_Enough":
        case 6:
            message.result = 6;
            break;
        }
        return message;
    };

    /**
     * Creates a plain object from a WebRspPlayerCharge message. Also converts values to other types if specified.
     * @function toObject
     * @memberof WebRspPlayerCharge
     * @static
     * @param {WebRspPlayerCharge} message WebRspPlayerCharge
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    WebRspPlayerCharge.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults)
            object.result = options.enums === String ? "EN_Web_OK" : 1;
        if (message.result != null && message.hasOwnProperty("result"))
            object.result = options.enums === String ? $root.ENWebResult[message.result] : message.result;
        return object;
    };

    /**
     * Converts this WebRspPlayerCharge to JSON.
     * @function toJSON
     * @memberof WebRspPlayerCharge
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    WebRspPlayerCharge.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return WebRspPlayerCharge;
})();

/**
 * SSMsgType enum.
 * @exports SSMsgType
 * @enum {string}
 * @property {number} EN_SS_Request=1 EN_SS_Request value
 * @property {number} EN_SS_Response=2 EN_SS_Response value
 */
$root.SSMsgType = (function() {
    var valuesById = {}, values = Object.create(valuesById);
    values[valuesById[1] = "EN_SS_Request"] = 1;
    values[valuesById[2] = "EN_SS_Response"] = 2;
    return values;
})();

$root.PBSSMsg = (function() {

    /**
     * Properties of a PBSSMsg.
     * @exports IPBSSMsg
     * @interface IPBSSMsg
     * @property {number|Long|null} [uid] PBSSMsg uid
     * @property {SSMsgType|null} [type] PBSSMsg type
     * @property {ISSReqQuery|null} [ssReqQuery] PBSSMsg ssReqQuery
     * @property {ISSRspQuery|null} [ssRspQuery] PBSSMsg ssRspQuery
     * @property {ISSReqSave|null} [ssReqSave] PBSSMsg ssReqSave
     * @property {ISSRspSave|null} [ssRspSave] PBSSMsg ssRspSave
     * @property {IWebReqPlayerLogin|null} [webReqPlayerLogin] PBSSMsg webReqPlayerLogin
     * @property {IWebRspPlayerLogin|null} [webRspPlayerLogin] PBSSMsg webRspPlayerLogin
     * @property {IWebReqPlayerBuyIn|null} [webReqPlayerBuyIn] PBSSMsg webReqPlayerBuyIn
     * @property {IWebRspPlayerBuyIn|null} [webRspPlayerBuyIn] PBSSMsg webRspPlayerBuyIn
     * @property {IWebNotifyPlayerInfo|null} [webNotifyPlayerInfo] PBSSMsg webNotifyPlayerInfo
     * @property {IWebReqPlayerCharge|null} [webReqPlayerCharge] PBSSMsg webReqPlayerCharge
     * @property {IWebRspPlayerCharge|null} [webRspPlayerCharge] PBSSMsg webRspPlayerCharge
     */

    /**
     * Constructs a new PBSSMsg.
     * @exports PBSSMsg
     * @classdesc Represents a PBSSMsg.
     * @implements IPBSSMsg
     * @constructor
     * @param {IPBSSMsg=} [properties] Properties to set
     */
    function PBSSMsg(properties) {
        if (properties)
            for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                if (properties[keys[i]] != null)
                    this[keys[i]] = properties[keys[i]];
    }

    /**
     * PBSSMsg uid.
     * @member {number|Long} uid
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.uid = $util.Long ? $util.Long.fromBits(0,0,true) : 0;

    /**
     * PBSSMsg type.
     * @member {SSMsgType} type
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.type = 1;

    /**
     * PBSSMsg ssReqQuery.
     * @member {ISSReqQuery|null|undefined} ssReqQuery
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.ssReqQuery = null;

    /**
     * PBSSMsg ssRspQuery.
     * @member {ISSRspQuery|null|undefined} ssRspQuery
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.ssRspQuery = null;

    /**
     * PBSSMsg ssReqSave.
     * @member {ISSReqSave|null|undefined} ssReqSave
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.ssReqSave = null;

    /**
     * PBSSMsg ssRspSave.
     * @member {ISSRspSave|null|undefined} ssRspSave
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.ssRspSave = null;

    /**
     * PBSSMsg webReqPlayerLogin.
     * @member {IWebReqPlayerLogin|null|undefined} webReqPlayerLogin
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webReqPlayerLogin = null;

    /**
     * PBSSMsg webRspPlayerLogin.
     * @member {IWebRspPlayerLogin|null|undefined} webRspPlayerLogin
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webRspPlayerLogin = null;

    /**
     * PBSSMsg webReqPlayerBuyIn.
     * @member {IWebReqPlayerBuyIn|null|undefined} webReqPlayerBuyIn
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webReqPlayerBuyIn = null;

    /**
     * PBSSMsg webRspPlayerBuyIn.
     * @member {IWebRspPlayerBuyIn|null|undefined} webRspPlayerBuyIn
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webRspPlayerBuyIn = null;

    /**
     * PBSSMsg webNotifyPlayerInfo.
     * @member {IWebNotifyPlayerInfo|null|undefined} webNotifyPlayerInfo
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webNotifyPlayerInfo = null;

    /**
     * PBSSMsg webReqPlayerCharge.
     * @member {IWebReqPlayerCharge|null|undefined} webReqPlayerCharge
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webReqPlayerCharge = null;

    /**
     * PBSSMsg webRspPlayerCharge.
     * @member {IWebRspPlayerCharge|null|undefined} webRspPlayerCharge
     * @memberof PBSSMsg
     * @instance
     */
    PBSSMsg.prototype.webRspPlayerCharge = null;

    // OneOf field names bound to virtual getters and setters
    var $oneOfFields;

    /**
     * PBSSMsg msgUnion.
     * @member {"ssReqQuery"|"ssRspQuery"|"ssReqSave"|"ssRspSave"|"webReqPlayerLogin"|"webRspPlayerLogin"|"webReqPlayerBuyIn"|"webRspPlayerBuyIn"|"webNotifyPlayerInfo"|"webReqPlayerCharge"|"webRspPlayerCharge"|undefined} msgUnion
     * @memberof PBSSMsg
     * @instance
     */
    Object.defineProperty(PBSSMsg.prototype, "msgUnion", {
        get: $util.oneOfGetter($oneOfFields = ["ssReqQuery", "ssRspQuery", "ssReqSave", "ssRspSave", "webReqPlayerLogin", "webRspPlayerLogin", "webReqPlayerBuyIn", "webRspPlayerBuyIn", "webNotifyPlayerInfo", "webReqPlayerCharge", "webRspPlayerCharge"]),
        set: $util.oneOfSetter($oneOfFields)
    });

    /**
     * Creates a new PBSSMsg instance using the specified properties.
     * @function create
     * @memberof PBSSMsg
     * @static
     * @param {IPBSSMsg=} [properties] Properties to set
     * @returns {PBSSMsg} PBSSMsg instance
     */
    PBSSMsg.create = function create(properties) {
        return new PBSSMsg(properties);
    };

    /**
     * Encodes the specified PBSSMsg message. Does not implicitly {@link PBSSMsg.verify|verify} messages.
     * @function encode
     * @memberof PBSSMsg
     * @static
     * @param {IPBSSMsg} message PBSSMsg message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBSSMsg.encode = function encode(message, writer) {
        if (!writer)
            writer = $Writer.create();
        if (message.uid != null && message.hasOwnProperty("uid"))
            writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.uid);
        if (message.type != null && message.hasOwnProperty("type"))
            writer.uint32(/* id 2, wireType 0 =*/16).int32(message.type);
        if (message.ssReqQuery != null && message.hasOwnProperty("ssReqQuery"))
            $root.SSReqQuery.encode(message.ssReqQuery, writer.uint32(/* id 10000, wireType 2 =*/80002).fork()).ldelim();
        if (message.ssRspQuery != null && message.hasOwnProperty("ssRspQuery"))
            $root.SSRspQuery.encode(message.ssRspQuery, writer.uint32(/* id 10001, wireType 2 =*/80010).fork()).ldelim();
        if (message.ssReqSave != null && message.hasOwnProperty("ssReqSave"))
            $root.SSReqSave.encode(message.ssReqSave, writer.uint32(/* id 10002, wireType 2 =*/80018).fork()).ldelim();
        if (message.ssRspSave != null && message.hasOwnProperty("ssRspSave"))
            $root.SSRspSave.encode(message.ssRspSave, writer.uint32(/* id 10003, wireType 2 =*/80026).fork()).ldelim();
        if (message.webReqPlayerLogin != null && message.hasOwnProperty("webReqPlayerLogin"))
            $root.WebReqPlayerLogin.encode(message.webReqPlayerLogin, writer.uint32(/* id 10010, wireType 2 =*/80082).fork()).ldelim();
        if (message.webRspPlayerLogin != null && message.hasOwnProperty("webRspPlayerLogin"))
            $root.WebRspPlayerLogin.encode(message.webRspPlayerLogin, writer.uint32(/* id 10011, wireType 2 =*/80090).fork()).ldelim();
        if (message.webReqPlayerBuyIn != null && message.hasOwnProperty("webReqPlayerBuyIn"))
            $root.WebReqPlayerBuyIn.encode(message.webReqPlayerBuyIn, writer.uint32(/* id 10012, wireType 2 =*/80098).fork()).ldelim();
        if (message.webRspPlayerBuyIn != null && message.hasOwnProperty("webRspPlayerBuyIn"))
            $root.WebRspPlayerBuyIn.encode(message.webRspPlayerBuyIn, writer.uint32(/* id 10013, wireType 2 =*/80106).fork()).ldelim();
        if (message.webNotifyPlayerInfo != null && message.hasOwnProperty("webNotifyPlayerInfo"))
            $root.WebNotifyPlayerInfo.encode(message.webNotifyPlayerInfo, writer.uint32(/* id 10014, wireType 2 =*/80114).fork()).ldelim();
        if (message.webReqPlayerCharge != null && message.hasOwnProperty("webReqPlayerCharge"))
            $root.WebReqPlayerCharge.encode(message.webReqPlayerCharge, writer.uint32(/* id 10015, wireType 2 =*/80122).fork()).ldelim();
        if (message.webRspPlayerCharge != null && message.hasOwnProperty("webRspPlayerCharge"))
            $root.WebRspPlayerCharge.encode(message.webRspPlayerCharge, writer.uint32(/* id 10016, wireType 2 =*/80130).fork()).ldelim();
        return writer;
    };

    /**
     * Encodes the specified PBSSMsg message, length delimited. Does not implicitly {@link PBSSMsg.verify|verify} messages.
     * @function encodeDelimited
     * @memberof PBSSMsg
     * @static
     * @param {IPBSSMsg} message PBSSMsg message or plain object to encode
     * @param {$protobuf.Writer} [writer] Writer to encode to
     * @returns {$protobuf.Writer} Writer
     */
    PBSSMsg.encodeDelimited = function encodeDelimited(message, writer) {
        return this.encode(message, writer).ldelim();
    };

    /**
     * Decodes a PBSSMsg message from the specified reader or buffer.
     * @function decode
     * @memberof PBSSMsg
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @param {number} [length] Message length if known beforehand
     * @returns {PBSSMsg} PBSSMsg
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBSSMsg.decode = function decode(reader, length) {
        if (!(reader instanceof $Reader))
            reader = $Reader.create(reader);
        var end = length === undefined ? reader.len : reader.pos + length, message = new $root.PBSSMsg();
        while (reader.pos < end) {
            var tag = reader.uint32();
            switch (tag >>> 3) {
            case 1:
                message.uid = reader.uint64();
                break;
            case 2:
                message.type = reader.int32();
                break;
            case 10000:
                message.ssReqQuery = $root.SSReqQuery.decode(reader, reader.uint32());
                break;
            case 10001:
                message.ssRspQuery = $root.SSRspQuery.decode(reader, reader.uint32());
                break;
            case 10002:
                message.ssReqSave = $root.SSReqSave.decode(reader, reader.uint32());
                break;
            case 10003:
                message.ssRspSave = $root.SSRspSave.decode(reader, reader.uint32());
                break;
            case 10010:
                message.webReqPlayerLogin = $root.WebReqPlayerLogin.decode(reader, reader.uint32());
                break;
            case 10011:
                message.webRspPlayerLogin = $root.WebRspPlayerLogin.decode(reader, reader.uint32());
                break;
            case 10012:
                message.webReqPlayerBuyIn = $root.WebReqPlayerBuyIn.decode(reader, reader.uint32());
                break;
            case 10013:
                message.webRspPlayerBuyIn = $root.WebRspPlayerBuyIn.decode(reader, reader.uint32());
                break;
            case 10014:
                message.webNotifyPlayerInfo = $root.WebNotifyPlayerInfo.decode(reader, reader.uint32());
                break;
            case 10015:
                message.webReqPlayerCharge = $root.WebReqPlayerCharge.decode(reader, reader.uint32());
                break;
            case 10016:
                message.webRspPlayerCharge = $root.WebRspPlayerCharge.decode(reader, reader.uint32());
                break;
            default:
                reader.skipType(tag & 7);
                break;
            }
        }
        return message;
    };

    /**
     * Decodes a PBSSMsg message from the specified reader or buffer, length delimited.
     * @function decodeDelimited
     * @memberof PBSSMsg
     * @static
     * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
     * @returns {PBSSMsg} PBSSMsg
     * @throws {Error} If the payload is not a reader or valid buffer
     * @throws {$protobuf.util.ProtocolError} If required fields are missing
     */
    PBSSMsg.decodeDelimited = function decodeDelimited(reader) {
        if (!(reader instanceof $Reader))
            reader = new $Reader(reader);
        return this.decode(reader, reader.uint32());
    };

    /**
     * Verifies a PBSSMsg message.
     * @function verify
     * @memberof PBSSMsg
     * @static
     * @param {Object.<string,*>} message Plain object to verify
     * @returns {string|null} `null` if valid, otherwise the reason why it is not
     */
    PBSSMsg.verify = function verify(message) {
        if (typeof message !== "object" || message === null)
            return "object expected";
        var properties = {};
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (!$util.isInteger(message.uid) && !(message.uid && $util.isInteger(message.uid.low) && $util.isInteger(message.uid.high)))
                return "uid: integer|Long expected";
        if (message.type != null && message.hasOwnProperty("type"))
            switch (message.type) {
            default:
                return "type: enum value expected";
            case 1:
            case 2:
                break;
            }
        if (message.ssReqQuery != null && message.hasOwnProperty("ssReqQuery")) {
            properties.msgUnion = 1;
            {
                var error = $root.SSReqQuery.verify(message.ssReqQuery);
                if (error)
                    return "ssReqQuery." + error;
            }
        }
        if (message.ssRspQuery != null && message.hasOwnProperty("ssRspQuery")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.SSRspQuery.verify(message.ssRspQuery);
                if (error)
                    return "ssRspQuery." + error;
            }
        }
        if (message.ssReqSave != null && message.hasOwnProperty("ssReqSave")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.SSReqSave.verify(message.ssReqSave);
                if (error)
                    return "ssReqSave." + error;
            }
        }
        if (message.ssRspSave != null && message.hasOwnProperty("ssRspSave")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.SSRspSave.verify(message.ssRspSave);
                if (error)
                    return "ssRspSave." + error;
            }
        }
        if (message.webReqPlayerLogin != null && message.hasOwnProperty("webReqPlayerLogin")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebReqPlayerLogin.verify(message.webReqPlayerLogin);
                if (error)
                    return "webReqPlayerLogin." + error;
            }
        }
        if (message.webRspPlayerLogin != null && message.hasOwnProperty("webRspPlayerLogin")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebRspPlayerLogin.verify(message.webRspPlayerLogin);
                if (error)
                    return "webRspPlayerLogin." + error;
            }
        }
        if (message.webReqPlayerBuyIn != null && message.hasOwnProperty("webReqPlayerBuyIn")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebReqPlayerBuyIn.verify(message.webReqPlayerBuyIn);
                if (error)
                    return "webReqPlayerBuyIn." + error;
            }
        }
        if (message.webRspPlayerBuyIn != null && message.hasOwnProperty("webRspPlayerBuyIn")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebRspPlayerBuyIn.verify(message.webRspPlayerBuyIn);
                if (error)
                    return "webRspPlayerBuyIn." + error;
            }
        }
        if (message.webNotifyPlayerInfo != null && message.hasOwnProperty("webNotifyPlayerInfo")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebNotifyPlayerInfo.verify(message.webNotifyPlayerInfo);
                if (error)
                    return "webNotifyPlayerInfo." + error;
            }
        }
        if (message.webReqPlayerCharge != null && message.hasOwnProperty("webReqPlayerCharge")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebReqPlayerCharge.verify(message.webReqPlayerCharge);
                if (error)
                    return "webReqPlayerCharge." + error;
            }
        }
        if (message.webRspPlayerCharge != null && message.hasOwnProperty("webRspPlayerCharge")) {
            if (properties.msgUnion === 1)
                return "msgUnion: multiple values";
            properties.msgUnion = 1;
            {
                var error = $root.WebRspPlayerCharge.verify(message.webRspPlayerCharge);
                if (error)
                    return "webRspPlayerCharge." + error;
            }
        }
        return null;
    };

    /**
     * Creates a PBSSMsg message from a plain object. Also converts values to their respective internal types.
     * @function fromObject
     * @memberof PBSSMsg
     * @static
     * @param {Object.<string,*>} object Plain object
     * @returns {PBSSMsg} PBSSMsg
     */
    PBSSMsg.fromObject = function fromObject(object) {
        if (object instanceof $root.PBSSMsg)
            return object;
        var message = new $root.PBSSMsg();
        if (object.uid != null)
            if ($util.Long)
                (message.uid = $util.Long.fromValue(object.uid)).unsigned = true;
            else if (typeof object.uid === "string")
                message.uid = parseInt(object.uid, 10);
            else if (typeof object.uid === "number")
                message.uid = object.uid;
            else if (typeof object.uid === "object")
                message.uid = new $util.LongBits(object.uid.low >>> 0, object.uid.high >>> 0).toNumber(true);
        switch (object.type) {
        case "EN_SS_Request":
        case 1:
            message.type = 1;
            break;
        case "EN_SS_Response":
        case 2:
            message.type = 2;
            break;
        }
        if (object.ssReqQuery != null) {
            if (typeof object.ssReqQuery !== "object")
                throw TypeError(".PBSSMsg.ssReqQuery: object expected");
            message.ssReqQuery = $root.SSReqQuery.fromObject(object.ssReqQuery);
        }
        if (object.ssRspQuery != null) {
            if (typeof object.ssRspQuery !== "object")
                throw TypeError(".PBSSMsg.ssRspQuery: object expected");
            message.ssRspQuery = $root.SSRspQuery.fromObject(object.ssRspQuery);
        }
        if (object.ssReqSave != null) {
            if (typeof object.ssReqSave !== "object")
                throw TypeError(".PBSSMsg.ssReqSave: object expected");
            message.ssReqSave = $root.SSReqSave.fromObject(object.ssReqSave);
        }
        if (object.ssRspSave != null) {
            if (typeof object.ssRspSave !== "object")
                throw TypeError(".PBSSMsg.ssRspSave: object expected");
            message.ssRspSave = $root.SSRspSave.fromObject(object.ssRspSave);
        }
        if (object.webReqPlayerLogin != null) {
            if (typeof object.webReqPlayerLogin !== "object")
                throw TypeError(".PBSSMsg.webReqPlayerLogin: object expected");
            message.webReqPlayerLogin = $root.WebReqPlayerLogin.fromObject(object.webReqPlayerLogin);
        }
        if (object.webRspPlayerLogin != null) {
            if (typeof object.webRspPlayerLogin !== "object")
                throw TypeError(".PBSSMsg.webRspPlayerLogin: object expected");
            message.webRspPlayerLogin = $root.WebRspPlayerLogin.fromObject(object.webRspPlayerLogin);
        }
        if (object.webReqPlayerBuyIn != null) {
            if (typeof object.webReqPlayerBuyIn !== "object")
                throw TypeError(".PBSSMsg.webReqPlayerBuyIn: object expected");
            message.webReqPlayerBuyIn = $root.WebReqPlayerBuyIn.fromObject(object.webReqPlayerBuyIn);
        }
        if (object.webRspPlayerBuyIn != null) {
            if (typeof object.webRspPlayerBuyIn !== "object")
                throw TypeError(".PBSSMsg.webRspPlayerBuyIn: object expected");
            message.webRspPlayerBuyIn = $root.WebRspPlayerBuyIn.fromObject(object.webRspPlayerBuyIn);
        }
        if (object.webNotifyPlayerInfo != null) {
            if (typeof object.webNotifyPlayerInfo !== "object")
                throw TypeError(".PBSSMsg.webNotifyPlayerInfo: object expected");
            message.webNotifyPlayerInfo = $root.WebNotifyPlayerInfo.fromObject(object.webNotifyPlayerInfo);
        }
        if (object.webReqPlayerCharge != null) {
            if (typeof object.webReqPlayerCharge !== "object")
                throw TypeError(".PBSSMsg.webReqPlayerCharge: object expected");
            message.webReqPlayerCharge = $root.WebReqPlayerCharge.fromObject(object.webReqPlayerCharge);
        }
        if (object.webRspPlayerCharge != null) {
            if (typeof object.webRspPlayerCharge !== "object")
                throw TypeError(".PBSSMsg.webRspPlayerCharge: object expected");
            message.webRspPlayerCharge = $root.WebRspPlayerCharge.fromObject(object.webRspPlayerCharge);
        }
        return message;
    };

    /**
     * Creates a plain object from a PBSSMsg message. Also converts values to other types if specified.
     * @function toObject
     * @memberof PBSSMsg
     * @static
     * @param {PBSSMsg} message PBSSMsg
     * @param {$protobuf.IConversionOptions} [options] Conversion options
     * @returns {Object.<string,*>} Plain object
     */
    PBSSMsg.toObject = function toObject(message, options) {
        if (!options)
            options = {};
        var object = {};
        if (options.defaults) {
            if ($util.Long) {
                var long = new $util.Long(0, 0, true);
                object.uid = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
            } else
                object.uid = options.longs === String ? "0" : 0;
            object.type = options.enums === String ? "EN_SS_Request" : 1;
        }
        if (message.uid != null && message.hasOwnProperty("uid"))
            if (typeof message.uid === "number")
                object.uid = options.longs === String ? String(message.uid) : message.uid;
            else
                object.uid = options.longs === String ? $util.Long.prototype.toString.call(message.uid) : options.longs === Number ? new $util.LongBits(message.uid.low >>> 0, message.uid.high >>> 0).toNumber(true) : message.uid;
        if (message.type != null && message.hasOwnProperty("type"))
            object.type = options.enums === String ? $root.SSMsgType[message.type] : message.type;
        if (message.ssReqQuery != null && message.hasOwnProperty("ssReqQuery")) {
            object.ssReqQuery = $root.SSReqQuery.toObject(message.ssReqQuery, options);
            if (options.oneofs)
                object.msgUnion = "ssReqQuery";
        }
        if (message.ssRspQuery != null && message.hasOwnProperty("ssRspQuery")) {
            object.ssRspQuery = $root.SSRspQuery.toObject(message.ssRspQuery, options);
            if (options.oneofs)
                object.msgUnion = "ssRspQuery";
        }
        if (message.ssReqSave != null && message.hasOwnProperty("ssReqSave")) {
            object.ssReqSave = $root.SSReqSave.toObject(message.ssReqSave, options);
            if (options.oneofs)
                object.msgUnion = "ssReqSave";
        }
        if (message.ssRspSave != null && message.hasOwnProperty("ssRspSave")) {
            object.ssRspSave = $root.SSRspSave.toObject(message.ssRspSave, options);
            if (options.oneofs)
                object.msgUnion = "ssRspSave";
        }
        if (message.webReqPlayerLogin != null && message.hasOwnProperty("webReqPlayerLogin")) {
            object.webReqPlayerLogin = $root.WebReqPlayerLogin.toObject(message.webReqPlayerLogin, options);
            if (options.oneofs)
                object.msgUnion = "webReqPlayerLogin";
        }
        if (message.webRspPlayerLogin != null && message.hasOwnProperty("webRspPlayerLogin")) {
            object.webRspPlayerLogin = $root.WebRspPlayerLogin.toObject(message.webRspPlayerLogin, options);
            if (options.oneofs)
                object.msgUnion = "webRspPlayerLogin";
        }
        if (message.webReqPlayerBuyIn != null && message.hasOwnProperty("webReqPlayerBuyIn")) {
            object.webReqPlayerBuyIn = $root.WebReqPlayerBuyIn.toObject(message.webReqPlayerBuyIn, options);
            if (options.oneofs)
                object.msgUnion = "webReqPlayerBuyIn";
        }
        if (message.webRspPlayerBuyIn != null && message.hasOwnProperty("webRspPlayerBuyIn")) {
            object.webRspPlayerBuyIn = $root.WebRspPlayerBuyIn.toObject(message.webRspPlayerBuyIn, options);
            if (options.oneofs)
                object.msgUnion = "webRspPlayerBuyIn";
        }
        if (message.webNotifyPlayerInfo != null && message.hasOwnProperty("webNotifyPlayerInfo")) {
            object.webNotifyPlayerInfo = $root.WebNotifyPlayerInfo.toObject(message.webNotifyPlayerInfo, options);
            if (options.oneofs)
                object.msgUnion = "webNotifyPlayerInfo";
        }
        if (message.webReqPlayerCharge != null && message.hasOwnProperty("webReqPlayerCharge")) {
            object.webReqPlayerCharge = $root.WebReqPlayerCharge.toObject(message.webReqPlayerCharge, options);
            if (options.oneofs)
                object.msgUnion = "webReqPlayerCharge";
        }
        if (message.webRspPlayerCharge != null && message.hasOwnProperty("webRspPlayerCharge")) {
            object.webRspPlayerCharge = $root.WebRspPlayerCharge.toObject(message.webRspPlayerCharge, options);
            if (options.oneofs)
                object.msgUnion = "webRspPlayerCharge";
        }
        return object;
    };

    /**
     * Converts this PBSSMsg to JSON.
     * @function toJSON
     * @memberof PBSSMsg
     * @instance
     * @returns {Object.<string,*>} JSON object
     */
    PBSSMsg.prototype.toJSON = function toJSON() {
        return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
    };

    return PBSSMsg;
})();

module.exports = $root;
