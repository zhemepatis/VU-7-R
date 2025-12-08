using Microsoft.AspNetCore.Mvc;

[ApiController]
[Route("api/[controller]")]
public class TemperatureController : ControllerBase
{
    private static readonly List<TempReading> Data = [];

    [HttpPost]
    public IActionResult Post([FromBody] TempReading reading)
    {
        Console.WriteLine("POST request");

        Data.Add(reading);
        Console.WriteLine($"Received: {reading.Value}°C at {reading.Timestamp}");

        return Ok(new { status = "ok" });
    }

    [HttpGet]
    public IActionResult Get()
    {
        Console.WriteLine("GET request");
        return Ok(Data);
    }
}

public class TempReading
{
    public float Value { get; init; }
    public DateTime Timestamp { get; init; } = DateTime.Now;
}