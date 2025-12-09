namespace Temperature.API.Models;

public record GetTemperatureIntervalRequest(DateTimeOffset IntervalStart, DateTimeOffset IntervalEnd);