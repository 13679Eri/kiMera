// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'json_schema.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

JsonSchema _$JsonSchemaFromJson(Map<String, dynamic> json) => JsonSchema(
      mode: $enumDecode(_$InstrumentsEnumMap, json['mode']),
    );

Map<String, dynamic> _$JsonSchemaToJson(JsonSchema instance) =>
    <String, dynamic>{
      'mode': _$InstrumentsEnumMap[instance.mode]!,
    };

const _$InstrumentsEnumMap = {
  Instruments.piano: 'piano',
  Instruments.trumpet: 'trumpet',
  Instruments.flute: 'flute',
  Instruments.violin: 'violin',
};
