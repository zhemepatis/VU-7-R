namespace Temperature.API.Extensions;

public static partial class TemperatureRecordsLogExtensions
{
    [LoggerMessage(
        Level = LogLevel.Information,
        EventName = "GetRequest",
        EventId = 1,
        Message = "GET request was initiated on {timestamp}")]
    public static partial void LogGetRequest(this ILogger<TemperatureController> logger, DateTimeOffset timestamp);

    [LoggerMessage(
        Level = LogLevel.Information,
        EventName = "PostRequest",
        EventId = 2,
        Message = "POST request was initiated on {timestamp}")]
    public static partial void LogPostRequest(this ILogger<TemperatureController> logger, DateTimeOffset timestamp);
}