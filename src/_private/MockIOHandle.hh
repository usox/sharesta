<?hh // strict

namespace Usox\Sharesta\_Private;

use namespace HH\Lib\IO;

class MockIOHandle implements IO\ReadHandle {
  public function __construct(private string $contents) {}
  public function isEndOfFile(): bool {
    return false;
  }
  public function rawReadBlocking(?int $limit = null): string {
    if ($limit is nonnull) {
      throw new \InvalidOperationException(
        'This class is meant to be injected into tests. Do not use!',
      );
    }
    return $this->contents;
  }
  public function readAsync(
    ?int $_limit = null,
    ?float $_timeout = null,
  ): Awaitable<string> {
    throw new \InvalidOperationException(
      'This class is meant to be injected into tests. Do not use!',
    );
  }
  public function readLineAsync(
    ?int $_max_bytes = null,
    ?float $_timeout_seconds = null,
  ): Awaitable<string> {
    throw new \InvalidOperationException(
      'This class is meant to be injected into tests. Do not use!',
    );
  }

}
