#pragma once

namespace CmdProcess
{
	enum CommandUpdateElementType
	{
		Floor=0,
		CompoG,
		CompoM,
		Material,
		Joint,		
		GridLine,
		LevelZone,
		Collection,
        Building,
        Opening,
        WallOpening,
		Well,
		Trench,
		ReinforceBarSetting,
		ConstructionZone,
		MarkM,
		MarkG,
        Block,
        BlockInstance,
        BlockOutline,
		FloorRebar,
		RibM,
		Spacer,
		Prefix,
		SectionView,
		StoryMaterial,
		WallEndRebarLine,
		FormWorkType,
		Area,
		AreaTemplate,
		BuildingInfo,
		BuildingInfoColorData,

		rapCompoG=100,
		rapFloor,
		rapGroup,
		rapUnit,
		rapBarSet,
		rapStrip,

		Connection,
		Etc,
		
		Pile = 150,
		CustomSection = 200,

		rapRebarTemplate = 300,

		msFormWorkTemplate = 400,
		FormWorkDesignUnit = 1000,
		FormWorkColumnShapeInfo,
		FormWorkWallShapeInfo,
		FormWorkBrickWallShapeInfo,
		FormWorkBeamShapeInfo,
		FormWorkReverseBeamShapeInfo,
		FormWorkSlabShapeInfo,

		END,
	};
};
