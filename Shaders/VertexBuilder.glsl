Vertex VertexBuilder()
{
	Vertex vertex;
	const ivec3 index = ivec3(indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID],
						      indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID + 1],
							  indices[gl_InstanceCustomIndexEXT].i[3 * gl_PrimitiveID + 2]);

	const Vertex v0 = vertices[gl_InstanceCustomIndexEXT].v[index.x];
	const Vertex v1 = vertices[gl_InstanceCustomIndexEXT].v[index.y];
	const Vertex v2 = vertices[gl_InstanceCustomIndexEXT].v[index.z];

	const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);

	vertex.Position = v0.Position * barycentricCoords.x + v1.Position * barycentricCoords.y + v2.Position * barycentricCoords.z;
	vertex.Position = vec3(meshBuffer[gl_InstanceCustomIndexEXT].meshProperties.MeshTransform * vec4(vertex.Position, 1.0));

	vertex.UV = v0.UV * barycentricCoords.x + v1.UV * barycentricCoords.y + v2.UV * barycentricCoords.z;
	vertex.Normal = v0.Normal * barycentricCoords.x + v1.Normal * barycentricCoords.y + v2.Normal * barycentricCoords.z;
    vertex.Tangant = v0.Tangant * barycentricCoords.x + v1.Tangant * barycentricCoords.y + v2.Tangant * barycentricCoords.z;
	vertex.BiTangant = v0.BiTangant * barycentricCoords.x + v1.BiTangant * barycentricCoords.y + v2.BiTangant * barycentricCoords.z;
	vertex.Color = v0.Color * barycentricCoords.x + v1.Color * barycentricCoords.y + v2.Color * barycentricCoords.z;

	return vertex;
}