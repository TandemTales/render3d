
@property( !hlms_render_depth_only || (hlms_shadowcaster && (exponential_shadow_maps || hlms_shadowcaster_point)) )
	@piece( output_type )PS_OUTPUT@end
@end @property( !(!hlms_render_depth_only || (hlms_shadowcaster && (exponential_shadow_maps || hlms_shadowcaster_point))) )
	@piece( output_type )void@end
@end

@property( hlms_render_depth_only && !alpha_test && !hlms_alpha_hash && !hlms_shadows_esm )
	@set( hlms_disable_stage, 1 )
@end

@piece( DeclOutputType )
	struct PS_OUTPUT
	{
		@property( !hlms_shadowcaster )
			midf4 colour0	[[ color(@counter(rtv_target)) ]];
		@else
			@property( !hlms_render_depth_only )
				float colour0	[[ color(@counter(rtv_target)) ]];
			@end
			@property( hlms_render_depth_only )
				float colour0	[[ depth(any) ]];
			@end
		@end

		@insertpiece( ExtraOutputTypes )
	};
@end
