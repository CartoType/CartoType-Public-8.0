/*
cartotype_feature_info.h
Copyright (C) 2022 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <cartotype_types.h>
#include <cartotype_errors.h>
#include <string>

namespace CartoTypeCore
{

class MString;
class DataInputStream;
class DataOutputStream;
class MapObject;

/** Constructs a 15-bit feature type code from 3 lower-case letters. */
inline uint32_t constexpr FeatureTypeCode(int A,int B,int C)
    {
    return (char(A - 'a' + 1) << 10) | (char(B - 'a' + 1) << 5) | (char(C - 'a' + 1));
    }

/**
Every map object has a feature type.
The first 32 feature types are used for routable objects and their values are indexes into tables of speeds and weights in route profiles.
Other feature types are constructed from three-letter codes.

All feature type values must fit into 15 bits, and therefore must be in the range 0...32767.
*/
enum class FeatureType: uint16_t
    {
    Motorway = 0,
    MotorwayLink = 1,
    TrunkRoad = 2,
    TrunkLink = 3,
    PrimaryRoad = 4,
    PrimaryLink = 5,
    SecondaryRoad = 6,
    SecondaryLink = 7,
    TertiaryRoad = 8,
    UnclassifiedRoad = 9,
    ResidentialRoad = 10,
    Track = 11,
    ServiceRoad = 12,
    PedestrianRoad = 13,
    VehicularFerry = 14,
    PassengerFerry = 15,
    LivingStreet = 16,
    Cycleway = 17,
    Path = 18,
    Footway = 19,
    Bridleway = 20,
    Steps = 21,
    OtherRoad = 22,
    UnpavedRoad = 23,
    Railway = 24,
    LightRail = 25,
    Subway = 26,
    AerialWay = 27,
    SkiDownhill = 28,
    SkiNordic = 29,
    Waterway = 30,
    UnknownRoute = 31,

    UnknownNonRoute = 32,

    /*
    Three-letter type codes. Keep these in alphabetic order of type letters to avoid duplicates.
    No routable types should appear among the three-letter codes; all routable objects should be
    given one of the types 0...31.

    The lowest possible three-letter code, aaa, has the value 1057 = 1024 + 32 + 1.
    The highest code, zzz, has the value 27482 = (1024 * 26) + (32 * 26) + 26.
    */
    Aerodrome = FeatureTypeCode('a','e','r'),
    Airport = FeatureTypeCode('a','i','r'),
    AirportGate = FeatureTypeCode('a','g','t'),
    AirportHoldingPosition = FeatureTypeCode('a','h','p'),
    Allotments = FeatureTypeCode('a','l','l'),
    AlpineHut = FeatureTypeCode('a','l','p'),
    AmbulanceStation = FeatureTypeCode('a','m','b'),
    Apron = FeatureTypeCode('a','p','r'),
    AerialWayPylon = FeatureTypeCode('a','p','y'),
    Artwork = FeatureTypeCode('a','r','t'),
    AerialWayStation = FeatureTypeCode('a','s','t'),
    AirTerminal = FeatureTypeCode('a','t','e'),
    Atm = FeatureTypeCode('a','t','m'),
    Attraction = FeatureTypeCode('a','t','t'),
    Bar = FeatureTypeCode('b','a','r'),
    Basin = FeatureTypeCode('b','a','s'),
    Bay = FeatureTypeCode('b','a','y'),
    Beach = FeatureTypeCode('b','e','a'),
    Beacon = FeatureTypeCode('b','e','c'),
    BedAndBreakfast = FeatureTypeCode('b','e','d'),
    Bench = FeatureTypeCode('b','e','n'),
    Bank = FeatureTypeCode('b','n','k'),
    Boatyard = FeatureTypeCode('b','o','a'),
    Borough = FeatureTypeCode('b','o','r'),
    Boundary = FeatureTypeCode('b','o','u'),
    BicycleParking = FeatureTypeCode('b','p','k'),
    BicycleRental = FeatureTypeCode('b','r','e'),
    BareRock = FeatureTypeCode('b','r','c'),
    Barracks = FeatureTypeCode('b','r','k'),
    BrownField = FeatureTypeCode('b','r','o'),
    BusStop = FeatureTypeCode('b','s','p'),
    BusStation = FeatureTypeCode('b','s','t'),
    BufferStop = FeatureTypeCode('b','u','f'),
    Building = FeatureTypeCode('b','u','i'),
    Bunker = FeatureTypeCode('b','u','n'),
    Cabin = FeatureTypeCode('c','a','b'),
    Cafe = FeatureTypeCode('c','a','f'),
    CampSite = FeatureTypeCode('c','a','m'),
    Canal = FeatureTypeCode('c','a','n'),
    CaveEntrance = FeatureTypeCode('c','a','v'),
    CableCar = FeatureTypeCode('c','c','r'),
    CableDistributionCabinet = FeatureTypeCode('c','d','c'),
    Cemetery = FeatureTypeCode('c','e','m'),
    ChairLift = FeatureTypeCode('c','h','a'),
    CheckPoint = FeatureTypeCode('c','h','e'),
    Chalet = FeatureTypeCode('c','h','l'),
    CivilBoundary = FeatureTypeCode('c','i','b'),
    Cinema = FeatureTypeCode('c','i','n'),
    City = FeatureTypeCode('c','i','t'),
    Cliff = FeatureTypeCode('c','l','f'),
    Clinic = FeatureTypeCode('c','l','i'),
    Commercial = FeatureTypeCode('c','m','r'),
    Coastline = FeatureTypeCode('c','o','a'),
    College = FeatureTypeCode('c','o','l'),
    Common = FeatureTypeCode('c','o','m'),
    Construction = FeatureTypeCode('c','n','s'),
    Conservation = FeatureTypeCode('c','n','v'),
    Continent = FeatureTypeCode('c','o','n'),
    County = FeatureTypeCode('c','o','u'),
    CarPark = FeatureTypeCode('c','p','k'),
    CarRental = FeatureTypeCode('c','r','e'),
    Crossing = FeatureTypeCode('c','r','o'),
    Country = FeatureTypeCode('c','r','y'),
    CarSharing = FeatureTypeCode('c','s','h'),
    CutLine = FeatureTypeCode('c','u','t'),
    CarWash = FeatureTypeCode('c','w','a'),
    CaravanSite = FeatureTypeCode('c','v','n'),
    CyclingRoute = FeatureTypeCode('c','y','r'),
    Dam = FeatureTypeCode('d','a','m'),
    DangerArea = FeatureTypeCode('d','a','n'),
    Dentist = FeatureTypeCode('d','e','n'),
    Disused = FeatureTypeCode('d','i','s'),
    Ditch = FeatureTypeCode('d','i','t'),
    Dock = FeatureTypeCode('d','o','c'),
    DogPark = FeatureTypeCode('d','o','g'),
    Drain = FeatureTypeCode('d','r','a'),
    DragLift = FeatureTypeCode('d','r','g'),
    Doctors = FeatureTypeCode('d','r','s'),
    District = FeatureTypeCode('d','s','t'),
    ElectricVehicleCharging = FeatureTypeCode('e','v','c'),
    Farm = FeatureTypeCode('f','a','r'),
    FarmYard = FeatureTypeCode('f','a','y'),
    Fell = FeatureTypeCode('f','e','l'),
    FerryTerminal = FeatureTypeCode('f','e','t'),
    FastFood = FeatureTypeCode('f','f','d'),
    FireExtinguisher = FeatureTypeCode('f','i','e'),
    FireFlapper = FeatureTypeCode('f','i','f'),
    FireHose = FeatureTypeCode('f','i','h'),
    FireStation = FeatureTypeCode('f','i','s'),
    FitnessStation = FeatureTypeCode('f','i','t'),
    FireHydrant = FeatureTypeCode('f','i','y'),
    Forestry = FeatureTypeCode('f','o','r'),
    Fishing = FeatureTypeCode('f','s','h'),
    Fuel = FeatureTypeCode('f','u','e'),
    Funicular = FeatureTypeCode('f','u','n'),
    Garages = FeatureTypeCode('g','a','r'),
    Gate = FeatureTypeCode('g','a','t'),
    Garden = FeatureTypeCode('g','d','n'),
    Generator = FeatureTypeCode('g','e','n'),
    GreenHouse = FeatureTypeCode('g','h','o'),
    Glacier = FeatureTypeCode('g','l','a'),
    GolfCourse = FeatureTypeCode('g','o','l'),
    Gondola = FeatureTypeCode('g','o','n'),
    GoodsAerialWay = FeatureTypeCode('g','o','o'),
    Grass = FeatureTypeCode('g','r','a'),
    GreenField = FeatureTypeCode('g','r','e'),
    GritBin = FeatureTypeCode('g','r','b'),
    GraveYard = FeatureTypeCode('g','r','y'),
    GuestHouse = FeatureTypeCode('g','u','e'),
    Halt = FeatureTypeCode('h','a','l'),
    Hamlet = FeatureTypeCode('h','a','m'),
    Hangar = FeatureTypeCode('h','a','n'),
    Heath = FeatureTypeCode('h','e','a'),
    Helipad = FeatureTypeCode('h','e','l'),
    RailwayHalt = FeatureTypeCode('h','l','t'),
    HikingRoute = FeatureTypeCode('h','i','k'),
    HorseRiding = FeatureTypeCode('h','o','r'),
    Hospital = FeatureTypeCode('h','o','s'),
    Hotel = FeatureTypeCode('h','o','t'),
    HorseRidingRoute = FeatureTypeCode('h','r','r'),
    Hostel = FeatureTypeCode('h','s','t'),
    IceRink = FeatureTypeCode('i','c','e'),
    Industrial = FeatureTypeCode('i','n','d'),
    Information = FeatureTypeCode('i','n','f'),
    Island = FeatureTypeCode('i','s','l'),
    IsolatedDwelling = FeatureTypeCode('i','s','o'),
    Junction = FeatureTypeCode('j','c','t'),
    Kindergarten = FeatureTypeCode('k','i','n'),
    LandFill = FeatureTypeCode('l','a','n'),
    Land = FeatureTypeCode('l','n','d'),
    LevelCrossing = FeatureTypeCode('l','e','v'),
    Library = FeatureTypeCode('l','i','b'),
    Locality = FeatureTypeCode('l','o','c'),
    LockGate = FeatureTypeCode('l','o','k'),
    MaritimeBoundary = FeatureTypeCode('m','a','b'),
    Mall = FeatureTypeCode('m','a','l'),
    Marsh = FeatureTypeCode('m','a','r'),
    Meadow = FeatureTypeCode('m','e','a'),
    Military = FeatureTypeCode('m','i','l'),
    Marina = FeatureTypeCode('m','n','a'),
    Motel = FeatureTypeCode('m','o','t'),
    MinorPowerLine = FeatureTypeCode('m','p','l'),
    MiniatureGolf = FeatureTypeCode('m','r','g'),
    MiniatureRailway = FeatureTypeCode('m','r','y'),
    Mud = FeatureTypeCode('m','u','d'),
    Municipality = FeatureTypeCode('m','u','n'),
    Museum = FeatureTypeCode('m','u','s'),
    NatureReserve = FeatureTypeCode('n','a','t'),
    NationalPark = FeatureTypeCode('n','a','p'),
    NavalBase = FeatureTypeCode('n','a','v'),
    Neighborhood = FeatureTypeCode('n','e','i'),
    NursingHome = FeatureTypeCode('n','u','r'),
    Orchard = FeatureTypeCode('o','r','c'),
    PrecisionApproachPathIndicator = FeatureTypeCode('p','a','p'),
    Park = FeatureTypeCode('p','a','r'),
    PublicBuilding = FeatureTypeCode('p','b','u'),
    PostBox = FeatureTypeCode('p','b','x'),
    PicnicTable = FeatureTypeCode('p','c','t'),
    Peak = FeatureTypeCode('p','e','a'),
    Pharmacy = FeatureTypeCode('p','h','a'),
    Phone = FeatureTypeCode('p','h','o'),
    PicnicSite = FeatureTypeCode('p','i','c'),
    Pier = FeatureTypeCode('p','i','e'),
    Pipeline = FeatureTypeCode('p','i','p'),
    ParkingEntrance = FeatureTypeCode('p','k','e'),
    Parking = FeatureTypeCode('p','k','g'),
    ParkingSpace = FeatureTypeCode('p','k','s'),
    PlantNursery = FeatureTypeCode('p','l','a'),
    Platform = FeatureTypeCode('p','l','f'),
    PlaceOfWorship = FeatureTypeCode('p','l','w'),
    Playground = FeatureTypeCode('p','l','y'),
    PostOffice = FeatureTypeCode('p','o','f'),
    Police = FeatureTypeCode('p','o','l'),
    ProtectedArea = FeatureTypeCode('p','r','a'),
    PowerSubStation = FeatureTypeCode('p','s','s'),
    Pub = FeatureTypeCode('p','u','b'),
    PowerLine = FeatureTypeCode('p','w','l'),
    PowerStation = FeatureTypeCode('p','w','s'),
    Quarry = FeatureTypeCode('q','a','r'),
    Range = FeatureTypeCode('r','a','n'),
    Rapids = FeatureTypeCode('r','a','p'),
    Recycling = FeatureTypeCode('r','c','y'),
    RecreationGround = FeatureTypeCode('r','e','c'),
    Reef = FeatureTypeCode('r','e','e'),
    Region = FeatureTypeCode('r','e','g'),
    Retail = FeatureTypeCode('r','e','t'),
    Ridge = FeatureTypeCode('r','i','j'),
    River = FeatureTypeCode('r','i','v'),
    Rock = FeatureTypeCode('r','o','c'),
    RoundHouse = FeatureTypeCode('r','o','u'),
    ResidentialArea = FeatureTypeCode('r','s','d'),
    Restaurant = FeatureTypeCode('r','s','t'),
    Reservoir = FeatureTypeCode('r','s','v'),
    Runway = FeatureTypeCode('r','u','n'),
    Route = FeatureTypeCode('r','u','t'),
    RiverBank = FeatureTypeCode('r','v','b'),
    SaltPond = FeatureTypeCode('s','a','l'),
    Sand = FeatureTypeCode('s','a','n'),
    Sauna = FeatureTypeCode('s','a','u'),
    School = FeatureTypeCode('s','c','h'),
    Scree = FeatureTypeCode('s','c','r'),
    Scrub = FeatureTypeCode('s','c','b'),
    Sea = FeatureTypeCode('s','e','a'),
    StateEmergencyServiceStation = FeatureTypeCode('s','e','s'),
    Shop = FeatureTypeCode('s','h','o'),
    SkiRoute = FeatureTypeCode('s','k','r'),
    Slipway = FeatureTypeCode('s','l','i'),
    SportsCenter = FeatureTypeCode('s','p','o'),
    SportsPitch = FeatureTypeCode('s','p','p'),
    Spring = FeatureTypeCode('s','p','r'),
    SportsTrack = FeatureTypeCode('s','p','t'),
    State = FeatureTypeCode('s','t','a'),
    Stadium = FeatureTypeCode('s','t','m'),
    RailwayStation = FeatureTypeCode('s','t','n'),
    Station = FeatureTypeCode('s','t','n'), // deliberate duplicate
    Stone = FeatureTypeCode('s','t','o'),
    StopPosition = FeatureTypeCode('s','t','p'),
    Stream = FeatureTypeCode('s','t','r'),
    Strait = FeatureTypeCode('s','t','t'),
    Suburb = FeatureTypeCode('s','u','b'),
    Supermarket = FeatureTypeCode('s','u','p'),
    SurveyPoint = FeatureTypeCode('s','u','r'),
    SubwayEntrance = FeatureTypeCode('s','w','e'),
    SwimmingPool = FeatureTypeCode('s','w','i'),
    Tank = FeatureTypeCode('t','a','n'),
    Taxi = FeatureTypeCode('t','a','x'),
    Theatre = FeatureTypeCode('t','h','e'),
    ThemePark = FeatureTypeCode('t','h','p'),
    Toilet = FeatureTypeCode('t','o','i'),
    Town = FeatureTypeCode('t','o','w'),
    TurningCircle = FeatureTypeCode('t','c','i'),
    TurningPoint = FeatureTypeCode('t','p','t'),
    Tram = FeatureTypeCode('t','r','a'),
    Tree = FeatureTypeCode('t','r','e'),
    TrafficSignals = FeatureTypeCode('t','r','f'),
    TrackPoint = FeatureTypeCode('t','r','p'),
    TreeRow = FeatureTypeCode('t','r','r'),
    TramStop = FeatureTypeCode('t','r','s'),
    TurnTable = FeatureTypeCode('t','u','r'),
    Tower = FeatureTypeCode('t','w','r'),
    Taxiway = FeatureTypeCode('t','w','y'),
    University = FeatureTypeCode('u','n','i'),
    VisualApproachSlopeIndicator = FeatureTypeCode('v','a','s'),
    VillageGreen = FeatureTypeCode('v','i','g'),
    Village = FeatureTypeCode('v','i','l'),
    Vineyard = FeatureTypeCode('v','i','n'),
    ViewPoint = FeatureTypeCode('v','i','w'),
    Volcano = FeatureTypeCode('v','o','l'),
    Waterfall = FeatureTypeCode('w','a','f'),
    WaterPark = FeatureTypeCode('w','a','p'),
    Water = FeatureTypeCode('w','a','t'),
    Weir = FeatureTypeCode('w','e','r'),
    Wetland = FeatureTypeCode('w','e','t'),
    Windsock = FeatureTypeCode('w','i','s'),
    WalkingRoute = FeatureTypeCode('w','l','k'),
    Wood = FeatureTypeCode('w','o','o'),
    Works = FeatureTypeCode('w','o','r'),
    Waypoint = FeatureTypeCode('w','p','t'),
    WaterTower = FeatureTypeCode('w','t','t'),
    WaterWell = FeatureTypeCode('w','t','w'),
    Zoo = FeatureTypeCode('z','o','o'),

    // legacy types for backward compatibility
    LegacyBridleway = FeatureTypeCode('b','r','i'),
    LegacyCycleway = FeatureTypeCode('c','y','c'),
    LegacyFootway = FeatureTypeCode('f','o','o'),
    LegacyLightRail = FeatureTypeCode('l','i','r'),
    LegacyMonorail = FeatureTypeCode('m','o','n'),
    LegacyNarrowGauge = FeatureTypeCode('n','a','r'),
    LegacyPreservedRailway = FeatureTypeCode('p','r','y'),
    LegacyRailway = FeatureTypeCode('r','l','y'),
    LegacySteps = FeatureTypeCode('s','t','e'),
    LegacySubway = FeatureTypeCode('s','w','y'),
    LegacyTram = FeatureTypeCode('t','r','a'),

    Invalid = INT16_MAX
    };

/** The feature discriminator stored in a FeatureInfo object. */
enum class FeatureDiscriminator
    {
    /** Not a route. */
    NonRoute = 0,
    /** A route that is a bridge. */
    BridgeRoute = 1,
    /** A route that is a tunnel. */
    TunnelRoute = 2,
    /** A route that is neither a tunnel nor a bridge. */
    Route = 3
    };

/**
Feature information for a map object, represented internally as a 32-bit value.

Every map object has a feature info value. Feature info values fall into two categories: route and non-route.

Route values have a feature type in the range 0...31 (Motorway...UnknownRoute); it is used as the index into the speed and bonus
arrays in a route profile. Route values have other information relevant to routing including the speed limit
and access restrictions.

Non-route values have a feature type in the range 32...32767. Non-route feature types, apart from the special values
UnknownNonRoute (32) and Invalid (32767), may be expressed as three-letter mnemonics, which can be constructed using
the FeatureTypeCode function. Non-route values have an 11-bit sub-type that may be used for fine distinctions such
as settlement rank or boundary type.

Both route and non-route values have a level in the range -8...7, where 0 represents ground level. The level is used as
one of the criteria determining drawing order.
*/
class FeatureInfo
    {
    private:
    template<typename T> constexpr Result SetValue(bool aMustBeRoute,uint32_t aMask,uint32_t aShift,T aValue)
        {
        if (Route() == aMustBeRoute)
            {
            m_value &= ~aMask;
            m_value |= ((uint32_t(aValue) << aShift) & aMask);
            return KErrorNone;
            }
        return aMustBeRoute ? KErrorFeatureInfoIsNotRoute : KErrorFeatureInfoIsRoute;
        }

    public:
    /** Creates a feature info object of type UnknownNonRoute. */
    constexpr FeatureInfo(): m_value(uint32_t(FeatureType::UnknownNonRoute) << KTypeShift) { }
    FeatureInfo(FeatureType aType);
    FeatureInfo(FeatureType aType,uint32_t aSubType);
    explicit FeatureInfo(uint32_t aSubType);
    /** The equality operator. */
    constexpr bool operator==(const FeatureInfo& aOther) const { return m_value == aOther.m_value; }
    /** The inequality operator. */
    constexpr bool operator!=(const FeatureInfo& aOther) const { return m_value != aOther.m_value; }
    /** The less-than operator. */
    constexpr bool operator<(const FeatureInfo& aOther) const { return m_value < aOther.m_value; }
    /** Returns the feature discriminator. */
    constexpr FeatureDiscriminator Discriminator() const { return FeatureDiscriminator(m_value & KDiscriminatorMask); }
    /** Returns true if this is a route. */
    constexpr bool Route() const { return (m_value & KDiscriminatorMask) != KDiscriminatorNonRoute; }
    /** Returns true if this is a route that is a tunnel. */
    constexpr bool Tunnel() const { return (m_value & KDiscriminatorMask) == KDiscriminatorTunnel; }
    /** Makes this object a tunnel or an ordinary route. Does nothing and returns an error if this is not a route. */
    constexpr Result SetTunnel(bool aValue) { return SetValue(true,KDiscriminatorMask,0,aValue ? KDiscriminatorTunnel : KDiscriminatorRoute); }
    /** Returns true if this is a route that is a bridge. */
    constexpr bool Bridge() const { return (m_value & KDiscriminatorMask) == KDiscriminatorBridge; }
    /** Makes this object a bridge or an ordinary route. Does nothing and returns an error if this is not a route. */
    constexpr Result SetBridge(bool aValue) { return SetValue(true,KDiscriminatorMask,0,aValue ? KDiscriminatorBridge : KDiscriminatorRoute); }
    /** Returns the level, which is in the range -8 ... 7. */
    constexpr int Level() const { int level = (m_value & KLevelMask) >> KLevelShift; return level > 7 ? level - 16 : level; }
    /** Sets the level, clamping it to the range -8 ... 7. */
    constexpr void SetLevel(int aLevel)
        {
        if (aLevel < -8)
            aLevel = -8;
        else if (aLevel > 7)
            aLevel = 7;
        m_value = (m_value & ~KLevelMask) | ((aLevel << KLevelShift) & KLevelMask);
        }
    /** Returns the feature type. */
    constexpr FeatureType Type() const { return FeatureType((m_value & (Route() ? KRouteTypeMask : KNonRouteTypeMask)) >> KTypeShift); }
    Result SetType(FeatureType aType);
    /** Returns true if this is a one-way route. */
    constexpr bool OneWay() const { return RouteFlag(KRouteOneWayFlag); }
    /** Returns true if this is a one-way route in the direction in which the route is stored. */
    constexpr bool OneWayForward() const { return Value(true,KRouteOneWayFlag | KRouteAccessWrongWayFlag,0) == KRouteOneWayFlag; }
    /** Returns true if this is a one-way route in the direction in which the route is stored. */
    constexpr bool OneWayBackward() const { return Value(true,KRouteOneWayFlag | KRouteAccessWrongWayFlag,0) == (KRouteOneWayFlag | KRouteAccessWrongWayFlag); }
    /** Sets whether this route is one-way. Does nothing and returns an error if this is not a route. */
    constexpr Result SetOneWay(bool aValue) { return SetRouteFlag(KRouteOneWayFlag,aValue); }
    /** Returns true if the rule of the road is to drive on the left. */
    constexpr bool DriveOnLeft() const { return RouteFlag(KRouteDriveOnLeftFlag); }
    /** Returns true if the rule of the road is to drive on the right. */
    constexpr bool DriveOnRight() const { return !RouteFlag(KRouteDriveOnLeftFlag); }
    /** Sets the rule of the road. Does nothing and returns an error if this is not a route. */
    constexpr Result SetDriveOnLeft(bool aValue) { return SetRouteFlag(KRouteDriveOnLeftFlag,aValue); }
    /** Reverses the one-way direction if this is a one-way route. */
    constexpr void ReverseOneWayDirection() { if (OneWay()) m_value ^= KRouteAccessWrongWayFlag; }
    /** Returns true if this is a route that is part of a roundabout. */
    constexpr bool Roundabout() const { return RouteFlag(KRouteRoundaboutFlag); }
    /** Sets whether this route is part of a roundabout. Does nothing and returns an error if this is not a route. */
    constexpr Result SetRoundabout(bool aValue) { return SetRouteFlag(KRouteRoundaboutFlag,aValue); }
    /** Returns true if this is a toll route. */
    constexpr bool Toll() const { return RouteFlag(KRouteTollFlag); }
    /** Sets whether this route is a toll route. Does nothing and returns an error if this is not a route. */
    constexpr Result SetToll(bool aValue) { return SetRouteFlag(KRouteTollFlag,aValue); }
    /** Returns true if this is a directed route arc going the wrong way along a one-way route. Useful only in route calculation. */
    constexpr bool WrongWay() const { return RouteFlag(KRouteAccessWrongWayFlag); }
    /** Sets whether this route is a directed route arc going the wrong way along a one-way route. Does nothing and returns an error if this is not a route. Useful only in route calculation. */
    constexpr Result SetWrongWay(bool aValue) { return SetRouteFlag(KRouteAccessWrongWayFlag,aValue); }
    /** Returns the speed limit in kph. A value of zero indicates that no speed limit is known. */
    constexpr uint32_t SpeedLimit() const { return Value(true,KRouteSpeedLimitMask,KRouteSpeedLimitShift); }
    /** Sets the speed limit in kph. A value of zero indicates that no speed limit is known. Does nothing and returns an error if this is not a route. */
    constexpr Result SetSpeedLimit(uint32_t aValue) { return SetValue(true,KRouteSpeedLimitMask,KRouteSpeedLimitShift,aValue); }
    /** Returns the gradient as a number in the range 0...7. Values 0...3 are uphill and 4...7 are downhill. Gradients are stored only in route arcs and not usually in other map objects. */
    constexpr uint32_t Gradient() const { return Value(true,KRouteGradientMask,KRouteGradientShift); }
    /** Sets the gradient. Does nothing and returns an error if this is not a route. Gradients are stored only in route arcs and not usually in other map objects. */
    constexpr Result SetGradient(uint32_t aValue) { return SetValue(true,KRouteGradientMask,KRouteGradientShift,aValue); }
    /** Returns the access flags as a combination of the KRouteAccess... values. */
    constexpr uint32_t Access() const { return Value(true,KRouteAccessMask,0); }
    /** Sets the access flags from a combination of the KRouteAccess... values. Does nothing and returns an error if this is not a route. */
    constexpr Result SetAccess(uint32_t aValue) { return SetValue(true,KRouteAccessMask,0,aValue); }
    /** Sets or clears the vehicle access flags. Does nothing and returns an error if this is not a route. */
    constexpr Result SetVehicleAccess(bool aValue) { return SetValue(true,KRouteAccessVehicle,0,aValue ? 0 : KRouteAccessVehicle); }
    /** Returns true if this is a route and pedestrian access is allowed. */
    constexpr bool PedestrianAccess() const { return !RouteFlag(KRouteAccessPedestrianFlag); }
    /** Allows or forbids pedestrian access. Does nothing and returns an error if this is not a route. */
    constexpr Result SetPedestrianAccess(bool aValue) { return SetRouteFlag(KRouteAccessPedestrianFlag,!aValue); }
    /** Returns true if this is a route and cycle access is allowed. */
    constexpr bool CycleAccess() const { return !RouteFlag(KRouteAccessCycleFlag); }
    /** Allows or forbids cycle access. Does nothing and returns an error if this is not a route. */
    constexpr Result SetCycleAccess(bool aValue) { return SetRouteFlag(KRouteAccessCycleFlag,!aValue); }
    /** Returns true if this is a route and motor vehicle access is allowed. */
    constexpr bool MotorVehicleAccess() const { return !RouteFlag(KRouteAccessMotorVehicleFlag); }
    /** Allows or forbids motor vehicle access. Does nothing and returns an error if this is not a route. */
    constexpr Result SetMotorVehicleAccess(bool aValue) { return SetRouteFlag(KRouteAccessMotorVehicleFlag,!aValue); }
    /** Returns true if this is a route and emergency vehicle access is allowed. */
    constexpr bool EmergencyVehicleAccess() const { return !RouteFlag(KRouteAccessEmergencyVehicleFlag); }
    /** Allows or forbids emergency vehicle access. Does nothing and returns an error if this is not a route. */
    constexpr Result SetEmergencyVehicleAccess(bool aValue) { return SetRouteFlag(KRouteAccessEmergencyVehicleFlag,!aValue); }
    /** Returns true if this is a route and there are access restrictions other than those specified by the standard flags. */
    constexpr bool OtherAccessRestricted() const { return RouteFlag(KRouteAccessOtherFlag); }
    /** Sets or clears the flag indicating that there are other access restrictions. Does nothing and returns an error if this is not a route. */
    constexpr Result SetOtherAccessRestricted(bool aValue) { return SetRouteFlag(KRouteAccessOtherFlag,aValue); }
    /** Returns true if this is a route and no normal access is allowed. */
    constexpr bool IsPrivate() const { return Route() ? ((m_value & KRouteAccessNormal) == KRouteAccessNormal) : false; }
    /** Returns the sub-type. Returns 0 if this is a route. */
    constexpr uint32_t SubType() const { return Value(false,KNonRouteSubTypeMask,KNonRouteSubTypeShift); }
    /** Sets the sub-type. Does nothing and returns an error if this is a route.*/
    constexpr Result SetSubType(uint32_t aValue) { return SetValue(false,KNonRouteSubTypeMask,KNonRouteSubTypeShift,aValue); }

    void Read(DataInputStream& aInput);
    void Write(DataOutputStream& aOutput) const;

    uint32_t WeightIndex() const;
    FeatureInfo ArcRouteInfo(bool aForwards) const;
    /** Returns the raw integer value. For internal use. */
    constexpr uint32_t RawValue() const { return m_value; }
    /** Creates a feature info object from a raw integer value. For internal use. */
    static constexpr FeatureInfo FromRawValue(uint32_t aRawValue) { FeatureInfo f; f.m_value = aRawValue; return f; }
    /** Creates a feature info object from a layer name, a set of string attributes, and a pre-CTM1-v7 integer attribute. For internal use only. */
    static FeatureInfo FromCTM1v7(const MString& aLayer,const MString& aStringAttribute,uint32_t aIntAttribute)
        {
        return FeatureInfo(aLayer,aStringAttribute,aIntAttribute);
        }
    /** Creates a feature info object from a road type mask, a set of string attributes, and a pre-CTM1-v7 integer attribute. For internal use only. */
    static FeatureInfo FromCTM1v7(uint32_t aRoadTypeMask,const MString& aStringAttribute,uint32_t aIntAttribute)
        {
        return FeatureInfo(aRoadTypeMask,aStringAttribute,aIntAttribute);
        }
    static FeatureInfo FromKArcFlags(uint32_t aArcFlags);

    /**
    Decides whether a road is a one-way motorway lane or a link on to or off the motorway;
    used in forbidding sharp turns from off-ramps to on-ramps.
    */
    constexpr bool MotorwayOrLink() const { return OneWay() && (Type() == FeatureType::Motorway ||
                                                      Type() == FeatureType::MotorwayLink ||
                                                      Type() == FeatureType::TrunkLink ||
                                                      Type() == FeatureType::PrimaryLink); }

    /** The number of feature types that are routes. They are types with the values 0...31, and those values are used as indexes into speed and bonus arrays in route profiles. */
    static constexpr uint32_t KRouteTypeCount = 32;
    /** The number of gradient types. */
    static constexpr uint32_t KGradientCount = 8;
    /** The maximum value of a sub-type of a non-route object. The minimum value is zero. */
    static constexpr uint32_t KMaxSubType = 2047;

    // The access constants are public because they are used in the vehicle type for routing.

    /** The amount by which the route access flags are shifted. */
    static constexpr uint32_t KRouteAccessShift = 26;
    /** A mask to select the route access flags. */
    static constexpr uint32_t KRouteAccessMask = 63U << KRouteAccessShift;
    /** A flag indicating that this route is one-way but is stored in the opposite direction to the allowed direction of travel. */
    static constexpr uint32_t KRouteAccessWrongWayFlag = 1 << 26;
    /** A flag to forbid pedestrian access in a feature info object, or, in a vehicle type, to indicate a pedestrian. */
    static constexpr uint32_t KRouteAccessPedestrianFlag = 1 << 27;
    /** A flag to forbid cycle access in a feature info object, or, in a vehicle type, to indicate a cyclist. */
    static constexpr uint32_t KRouteAccessCycleFlag = 1 << 28;
    /** A flag to forbid motor vehicle access in a feature info object, or, in a vehicle type, to indicate a motor vehicle. */
    static constexpr uint32_t KRouteAccessMotorVehicleFlag = 1 << 29;
    /** A flag to forbid emergency vehicle access in a feature info object, or, in a vehicle type, to indicate an emergency vehicle. */
    static constexpr uint32_t KRouteAccessEmergencyVehicleFlag = 1 << 30;
    /** A flag to indicate the presence of other access restrictions in a feature info object, or, in a vehicle type, to indicate a type of vehicle not covered by the other flags. */
    static constexpr uint32_t KRouteAccessOtherFlag = 1U << 31;
    /** A set of flags covering all vehicles. */
    static constexpr uint32_t KRouteAccessVehicle = KRouteAccessCycleFlag | KRouteAccessMotorVehicleFlag | KRouteAccessEmergencyVehicleFlag;
    /** A set of flags indicating normal access: that is, access for pedestrians, cyclists and motor vehicles. */
    static constexpr uint32_t KRouteAccessNormal = KRouteAccessCycleFlag | KRouteAccessMotorVehicleFlag | KRouteAccessPedestrianFlag;

    /** The Field type is used by SetField, which is intended for advanced and internal use only. */
    enum class Field
        {
        None,
        Discriminator,
        Level,
        Type,
        OneWayForward,
        OneWayBackward,
        DriveOnLeft,
        Roundabout,
        Toll,
        SpeedLimit,
        Gradient,
        Access,
        Vehicle,
        Pedestrian,
        Cycle,
        Motor,
        Emergency,
        AccessOther,
        SubType,
        RawValue
        };
    
    void SetField(Field aField,uint32_t aValue);
    uint32_t TunnelsBridgesAndDirectionSortValue() const;

    private:
    explicit FeatureInfo(const MString& aLayer,const MString& aStringAttribute,uint32_t aIntAttribute);
    explicit FeatureInfo(uint32_t aRoadTypeMask,const MString& aStringAttribute,uint32_t aIntAttribute);
    constexpr bool RouteFlag(uint32_t aFlag) const { return Route() && (m_value & aFlag) != 0; }
    constexpr Result SetRouteFlag(uint32_t aFlag,bool aValue)
        {
        if (Route())
            {
            if (aValue)
                m_value |= aFlag;
            else
                m_value &= ~aFlag;
            return KErrorNone;
            }
        return KErrorFeatureInfoIsNotRoute;
        }
    constexpr uint32_t Value(bool aMustBeRoute,uint32_t aMask,uint32_t aShift) const
        {
        if (Route() == aMustBeRoute)
            return (m_value & aMask) >> aShift;
        return 0;
        }
    void SetValueAndFixUp(bool aMustBeRoute,uint32_t aMask,uint32_t aShift,uint32_t aValue);
    void SetRouteFlagAndFixUp(uint32_t aFlag,uint32_t aValue);

    /*
    The feature information for a map object has two different internal
    structures, for route and non-route objects.

    The first 6 bits are the same in both types:

    A two-bit discriminator, with the values 0 (not a route), 1 (bridge), 2 (tunnel), 3 (route, neither bridge nor tunnel).
    The four-bit level (-8 to +7).

    Then, for routes:

    bits 6-10: the 5-bit route type (a feature type in the range 0...31).
    bit 11: one-way flag
    bit 12: drive-on-left flag
    bit 13: roundabout flag
    bit 14: toll flag
    bits 15-22: speed limit in kph
    bits 23-25: gradient
    bits 26-31: access flags (wrong-way, pedestrian, cycle, motor, emergency, other)

    OR, for non-route objects:

    bits 6-20: the 15-bit feature type
    bits 21-31: a sub-type (e.g., admin level, settlement rank)
    */

    static constexpr uint32_t KDiscriminatorMask = 3;
    static constexpr uint32_t KDiscriminatorNonRoute = 0;
    static constexpr uint32_t KDiscriminatorBridge = 1;
    static constexpr uint32_t KDiscriminatorTunnel = 2;
    static constexpr uint32_t KDiscriminatorRoute = 3;
    static constexpr uint32_t KLevelShift = 2;
    static constexpr uint32_t KLevelMask = 15 << KLevelShift;
    static constexpr uint32_t KTypeShift = 6;
    
    // route-specific constants
    static constexpr uint32_t KRouteTypeMask = 31 << KTypeShift;
    static constexpr uint32_t KRouteOneWayFlag = 1 << 11;
    static constexpr uint32_t KRouteDriveOnLeftFlag = 1 << 12;
    static constexpr uint32_t KRouteRoundaboutFlag = 1 << 13;
    static constexpr uint32_t KRouteTollFlag = 1 << 14;
    static constexpr uint32_t KRouteSpeedLimitShift = 15;
    static constexpr uint32_t KRouteSpeedLimitMask = 255 << KRouteSpeedLimitShift;
    static constexpr uint32_t KRouteGradientShift = 23;
    static constexpr uint32_t KRouteGradientMask = 7 << KRouteGradientShift;
    static constexpr uint32_t KRouteGradientDirectionFlag = 4 << KRouteGradientShift;

    // non-route-specific constants
    static constexpr uint32_t KNonRouteTypeMask = 32767 << KTypeShift;
    static constexpr uint32_t KNonRouteSubTypeShift = 21;
    static constexpr uint32_t KNonRouteSubTypeMask = 2047U << KNonRouteSubTypeShift;

    uint32_t m_value = 0;
    };

std::string FeatureTypeName(FeatureType aType);
FeatureType FeatureTypeFromName(const MString& aName);
bool FeatureTypeIsRoad(FeatureType aType);
bool FeatureTypeIsPath(FeatureType aType);
bool FeatureTypeIsLink(FeatureType aType);
FeatureType MajorRouteType(FeatureType aType);
const char* GradientName(int aGradient);
uint32_t GradientCode(int16_t aStartHeight,int16_t aEndHeight,int32_t aDistance);
uint32_t RoadTypeMask(const MString& aLayer);

} // namespace CartoTypeCore
