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
				'target_name': 'libsqlite3',
				'type': 'static_library',
				'sources':
				[
					'sqlite3secure.c',
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
				'target_name': 'sqlite3_test',
				'type': 'executable',
				'sources':
				[
					'main_test.c',
					'sqlite3secure.c'
				],
				'defines': [
					'SQLITE_HAS_CODEC',
				  ]
			},
			{
				'target_name': 'sqlite3',
				'type': 'executable',
				'sources':
				[
					'main.c',
					'sqlite3secure.c'
				],
				'defines': [
					'SQLITE_HAS_CODEC',
				  ]
			}
        ] #targets
	  }
    ]
  ]
}