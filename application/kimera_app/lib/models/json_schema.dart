import 'package:json_annotation/json_annotation.dart';

part 'json_schema.g.dart';

@JsonEnum()
enum Instruments { piano, trumpet, flute, violin }

@JsonSerializable()
class JsonSchema {
  /// The generated code assumes these values exist in JSON.
  final Instruments mode;

  /// The generated code below handles if the corresponding JSON value doesn't
  /// exist or is empty.
  //final DateTime? dateOfBirth;

  JsonSchema({required this.mode});

  /// Connect the generated [_$PersonFromJson] function to the `fromJson`
  /// factory.
  factory JsonSchema.fromJson(Map<String, dynamic> json) =>
      _$JsonSchemaFromJson(json);

  /// Connect the generated [_$PersonToJson] function to the `toJson` method.
  Map<String, dynamic> toJson() => _$JsonSchemaToJson(this);
}
