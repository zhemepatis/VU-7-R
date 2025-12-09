using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Temperature.API.Models;
using Temperature.API.Services;

[ApiController]
[Route("api/[controller]")]
public class TemperatureController : ControllerBase
{
    private ITemperatureRecordsService _temperatureRecordsService;

    public TemperatureController(ITemperatureRecordsService temperatureRecordsService)
    {
        _temperatureRecordsService = temperatureRecordsService;
    }

    [HttpGet]
    public async Task<IActionResult> Get([FromQuery] GetTemperatureIntervalRequest request)
    {
        var results = await _temperatureRecordsService.GetInterval(request);
        return Ok(results);
    }

    [HttpPost]
    public async Task<IActionResult> Post([FromBody] AddTemperatureRecordRequest request)
    {
        await _temperatureRecordsService.Add(request);
        return Ok(new { status = "ok" });
    }
}