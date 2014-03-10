{
'target_defaults': {
	'conditions':
			[
				['OS == "ios"',
			      {
			        'xcode_settings': {
			          'TARGETED_DEVICE_FAMILY': '1,2',
			          'CODE_SIGN_IDENTITY': 'iPhone Developer',
			          'IPHONEOS_DEPLOYMENT_TARGET': '5.0',
			          'ARCHS': '$(ARCHS_STANDARD_32_64_BIT)',
			        }, # xcode_settings
			      },
			    ],  # OS == "ios"
			]
},
'conditions': [
    ['OS == "ios"',
      {
        'xcode_settings': {
          'SDKROOT': 'iphoneos',
        }, # xcode_settings
        'targets': [
        	{
				'target_name': 'libaes',
				'type': 'static_library',
				'sources':
				[
					'aes_my.c',
					'aes.c'
				],
			}
        ]
      },
    ],  # OS == "ios"
    [
	'OS == "mac"',
	  {
		'targets': [
            {
				'target_name': 'aes',
				'type': 'executable',
				'sources':
				[
					'main.c',
					'aes.c'
				]
			}
        ] #targets
	  }
    ]
  ]
}