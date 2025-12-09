
using Microsoft.AspNetCore.Mvc;
using Temperature.API.Extensions;
using Temperature.API.Models;
using Temperature.API.Services;

[ApiController]
[Route("api/[controller]")]
public class TemperatureController : ControllerBase
{
    private ITemperatureRecordsService _temperatureRecordsService;
    private ILogger<TemperatureController> _logger;

    public TemperatureController(ITemperatureRecordsService temperatureRecordsService, ILogger<TemperatureController> logger)
    {
        _temperatureRecordsService = temperatureRecordsService;
        _logger = logger;
    }

    [HttpGet]
    public async Task<IActionResult> Get([FromQuery] GetTemperatureIntervalRequest request)
    {
        _logger.LogPostRequest(DateTimeOffset.Now);
        
        var results = await _temperatureRecordsService.GetInterval(request);
        return Ok(results);
    }

    [HttpPost]
    public async Task<IActionResult> Post([FromBody] AddTemperatureRecordRequest request)
    {
        _logger.LogPostRequest(DateTimeOffset.Now);

        await _temperatureRecordsService.Add(request);
        return Ok(new { status = "ok" });
    }
}