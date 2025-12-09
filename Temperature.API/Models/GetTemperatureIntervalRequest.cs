namespace Temperature.API.Models;

public record GetTemperatureIntervalRequest(DateTime IntervalStart, DateTime IntervalEnd);